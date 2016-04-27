/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * ofpmipv6.cc
 *
 *  Created on: Apr 10, 2016
 *      Author: ns3
 */

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/openflow-module.h"
#include "ns3/log.h"
#include "ns3/bridge-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("OpenFlowCsmaSwitch");

bool verbose = false;
bool use_drop = false;
ns3::Time timeout = ns3::Seconds (0);

bool
SetVerbose (std::string value)
{
      verbose = true;
        return true;
}

bool
SetDrop (std::string value)
{
      use_drop = true;
        return true;
}

bool
SetTimeout (std::string value)
{
      try {
                timeout = ns3::Seconds (atof (value.c_str ()));
                      return true;
                          }
        catch (...) { return false; }
          return false;
}

int
main(int argc,char *argv[])
{
//	#ifdef NS3_OPENFLOW

	std::string animFile = "ofpmipv6.xml"; //animation xml file

    CommandLine cmd;

    cmd.AddValue ("v", "Verbose (turns on logging).", MakeCallback (&SetVerbose));
    cmd.AddValue ("verbose", "Verbose (turns on logging).", MakeCallback (&SetVerbose));
    cmd.AddValue ("d", "Use Drop Controller (Learning if not specified).", MakeCallback (&SetDrop));
    cmd.AddValue ("drop", "Use Drop Controller (Learning if not specified).", MakeCallback (&SetDrop));
    cmd.AddValue ("t", "Learning Controller Timeout (has no effect if drop controller is specified).", MakeCallback ( &SetTimeout));
    cmd.AddValue ("timeout", "Learning Controller Timeout (has no effect if drop controller is specified).", MakeCallback ( &SetTimeout));
    cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);

    cmd.Parse (argc, argv);

    if (verbose)
    {
        LogComponentEnable ("OpenFlowCsmaSwitch", LOG_LEVEL_INFO);
        LogComponentEnable ("OpenFlowInterface", LOG_LEVEL_INFO);
        LogComponentEnable ("OpenFlowSwitchNetDevice", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    NS_LOG_INFO ("Create nodes");

    //Create physical devices
    NodeContainer terminals;
    terminals.Create(3);

    NodeContainer ofSwitch;
    ofSwitch.Create(2);

    //create a controller
    NodeContainer ofController;
    ofController.Create(1);

    //create p2p nodes, 2 pairs of AP + switch
    NodeContainer p2pNodes1;
    p2pNodes1.Create(1);
    p2pNodes1.Add(ofSwitch.Get(0));

    NodeContainer p2pNodes2;
    p2pNodes2.Create(1);
    p2pNodes2.Add(ofSwitch.Get(1));

    NodeContainer apNode;
    apNode.Add(p2pNodes1.Get(0));
    apNode.Add(p2pNodes2.Get(0));

    NS_LOG_INFO ("Create Topology");

    //AP and switch use point to point connection
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevice;
    p2pDevice = pointToPoint.Install(p2pNodes1);
    p2pDevice = pointToPoint.Install(p2pNodes2);

    //setup LAN, switch to controller to CN to AAA server
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate",DataRateValue(5000000));
    csma.SetChannelAttribute("Delay",TimeValue(MilliSeconds(2)));

    NetDeviceContainer correspondentNode;
    correspondentNode = csma.Install(terminals.Get(1)); //terminal 1 is CN, terminal 0 is MN, terminal 2 is AAA server.

    NetDeviceContainer aaaServer;
    aaaServer = csma.Install(terminals.Get(2));

    NetDeviceContainer switchDevice1,switchDevice2;
    switchDevice1 = csma.Install(ofSwitch.Get(0));
    switchDevice2 = csma.Install(ofSwitch.Get(1));

    NetDeviceContainer controllerDevice;
    controllerDevice = csma.Install(ofController);

    //setup wifi, AP to switches
    NodeContainer accessPoints0 = p2pNodes1.Get(0);
    NodeContainer accessPoints1 = p2pNodes2.Get(0);

    NodeContainer mobileNode = terminals.Get(0);  //terminal 0 is MN

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default();
    phy.SetChannel(channel.Create());

    WifiHelper wifi = WifiHelper::Default();
    wifi.SetRemoteStationManager("ns3::AarfWifiManager");

    NqosWifiMacHelper mac = NqosWifiMacHelper::Default();

    Ssid ssid = Ssid("ns-3-ssid");
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));

    NetDeviceContainer mobileDevice;
    mobileDevice = wifi.Install(phy, mac, mobileNode);

    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));

    NetDeviceContainer apDevices;
    apDevices = wifi.Install(phy, mac, accessPoints0);
    apDevices = wifi.Install(phy, mac, accessPoints1);

    //create mobility
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator", "MinX", DoubleValue(0.0),
    															"MinY", DoubleValue(0.0),
																"DeltaX", DoubleValue(5.0),
																"DeltaY", DoubleValue(1.0),
																"GridWidth", UintegerValue(5),
																"LayoutType", StringValue("RowFirst"));

    //set MN mobility
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(terminals.Get(0));
    terminals.Get(0) -> GetObject<MobilityModel>() -> SetPosition(Vector(0,0,0));
    terminals.Get(0) -> GetObject<ConstantVelocityMobilityModel>() -> SetVelocity(Vector(0.5,0,0));

    //set constant node mobility
    Ptr<ListPositionAllocator> ap0Alloc = CreateObject<ListPositionAllocator>();
    ap0Alloc -> Add(Vector(1,0,0));
    mobility.SetPositionAllocator(ap0Alloc);
    mobility.Install(accessPoints0);

    Ptr<ListPositionAllocator> ap1Alloc = CreateObject<ListPositionAllocator>();
    ap1Alloc -> Add(Vector(1,2,0));
    mobility.SetPositionAllocator(ap1Alloc);
    mobility.Install(accessPoints1);

    Ptr<ListPositionAllocator> switchAlloc = CreateObject<ListPositionAllocator>();
    switchAlloc -> Add(Vector(2,0,0));
    switchAlloc -> Add(Vector(2,2,0));
    mobility.SetPositionAllocator(switchAlloc);
    mobility.Install(ofSwitch);

    Ptr<ListPositionAllocator> controllerAlloc = CreateObject<ListPositionAllocator>();
    controllerAlloc -> Add(Vector(3,1,0));
    mobility.SetPositionAllocator(controllerAlloc);
    mobility.Install(ofController);

    Ptr<ListPositionAllocator> cnAlloc = CreateObject<ListPositionAllocator>();
    cnAlloc -> Add(Vector(4,0,0));
    mobility.SetPositionAllocator(cnAlloc);
    mobility.Install(terminals.Get(1));

    Ptr<ListPositionAllocator> aaaAlloc = CreateObject<ListPositionAllocator>();
    aaaAlloc -> Add(Vector(4,2,0));
    mobility.SetPositionAllocator(aaaAlloc);
    mobility.Install(terminals.Get(2));


//    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//    mobility.Install(accessPoints0);
//    mobility.Install(accessPoints1);
//    mobility.Install(terminals.Get(1));
//    mobility.Install(terminals.Get(2));
//    mobility.Install(ofSwitch);
//    mobility.Install(ofController);

    //create switch net device
    Ptr<Node> switchNode1 = ofSwitch.Get(0);
    Ptr<Node> switchNode2 = ofSwitch.Get(1);
    OpenFlowSwitchHelper switchHelper;
    if(use_drop){
    	Ptr<ns3::ofi::DropController> controller = CreateObject<ns3::ofi::DropController>();
    	switchHelper.Install(switchNode1, switchDevice1, controller);
    	switchHelper.Install(switchNode2, switchDevice2, controller);
    }else{
    	Ptr<ns3::ofi::LearningController> controller = CreateObject<ns3::ofi::LearningController>();
    	if(!timeout.IsZero()) controller -> SetAttribute("ExpirationTime", TimeValue(timeout));
    	switchHelper.Install(switchNode1, switchDevice1, controller);
    	switchHelper.Install(switchNode2, switchDevice2, controller);
    }

    //install internet stack
    InternetStackHelper stack;
    stack.SetIpv4StackInstall(false);  // install ipv6 only
    stack.Install(terminals);
    stack.Install(ofSwitch);
    stack.Install(ofController);
    stack.Install(accessPoints0);
    stack.Install(accessPoints1);

    //install IPv6 address
    NetDeviceContainer ipv6Device;
    ipv6Device = csma.Install(terminals);  //install ipv6 to terminals
    NetDeviceContainer ipv6Switch;
    ipv6Switch = csma.Install(ofSwitch);   //install ipv6 to ofSwitchs
    NetDeviceContainer ipv6Controller;
    ipv6Controller = csma.Install(ofController);   //install ipv6 to ofController
    NetDeviceContainer ipv6AP;
    ipv6AP = csma.Install(apNode);   //install ipv6 to apNode

    NS_LOG_INFO("Assign IPv6 Addresses.");
    Ipv6AddressHelper ipv6;
    ipv6.SetBase(Ipv6Address("2001:db8::"), Ipv6Prefix(64));

    Ipv6InterfaceContainer ipv6ICT;
//    ipv6IC.SetForwarding(0,true);  //set ipv6 node to router mode. default is host mode.
    ipv6ICT = ipv6.Assign(ipv6Device);  //use host mode for terminal.
    Ipv6InterfaceContainer ipv6ICS;
    ipv6ICS = ipv6.Assign(ipv6Switch);
    Ipv6InterfaceContainer ipv6ICC;
    ipv6ICC = ipv6.Assign(ipv6Controller);
    Ipv6InterfaceContainer ipv6ICA;
    ipv6ICA = ipv6.Assign(ipv6AP);

    NS_LOG_UNCOND("CN ipv6 address: "<<ipv6ICT.GetAddress(1,0));
    NS_LOG_UNCOND("MN ipv6 address: "<<ipv6ICT.GetAddress(0,0));
    NS_LOG_UNCOND("aaaServer ipv6 address: "<<ipv6ICT.GetAddress(2,0));
    NS_LOG_UNCOND("ofSwitch1 ipv6 address: "<<ipv6ICS.GetAddress(0,0));
    NS_LOG_UNCOND("ofSwitch2 ipv6 address: "<<ipv6ICS.GetAddress(1,0));
    NS_LOG_UNCOND("Controller ipv6 address: "<<ipv6ICC.GetAddress(0,0));
    NS_LOG_UNCOND("AP1 ipv6 address: "<<ipv6ICA.GetAddress(0,0));
    NS_LOG_UNCOND("AP2 ipv6 address: "<<ipv6ICA.GetAddress(1,0));

//    //set routing table
//    ipv6ICS.SetForwarding(0,true);
//    ipv6ICS.SetDefaultRouteInAllNodes(0);
//    ipv6ICS.SetForwarding(1,true);
//    ipv6ICS.SetDefaultRouteInAllNodes(1);
//    ipv6ICA.SetForwarding(0,true);
//    ipv6ICA.SetDefaultRouteInAllNodes(0);
//    ipv6ICA.SetForwarding(1,true);
//    ipv6ICA.SetDefaultRouteInAllNodes(1);

    //Create an OnOff application to send UDP datagrams from MN(0) to CN(1).
    NS_LOG_INFO("Create Applications.");
    uint16_t port = 9; //Discard port (RFC 863)

    OnOffHelper onoff ("ns3::UdpSocketFactory",
    					Address(Inet6SocketAddress(Ipv6Address(ipv6ICT.GetAddress(1,0)), port)));  //get CN address 1 and address index 0.
    onoff.SetConstantRate(DataRate("500kb/s"));
    //install application to MN(0) and setup start stop time
    ApplicationContainer app = onoff.Install(terminals.Get(0));
    app.Start(Seconds(1.0));
    app.Stop(Seconds(5.0));

    //Create an optional packet sink to receive these packets
    PacketSinkHelper sink("ns3::UdpSocketFactory",
    						Address(Inet6SocketAddress(Ipv6Address::GetAny(), port)));
    app = sink.Install(terminals.Get(1));
    app.Start(Seconds(0.0));
    app.Stop(Seconds(5.0));

    //Create a similar flow from MN(0) to AAAserver(2), to simulate the AAA request
    onoff.SetAttribute("Remote",
    					AddressValue(Inet6SocketAddress(Ipv6Address(ipv6ICT.GetAddress(2,0)), port)));
    app = onoff.Install(terminals.Get(0));
    app.Start(Seconds(0.5));
    app.Stop(Seconds(0.8));

    app = sink.Install(terminals.Get(2));
    app.Start(Seconds(0.0));
    app.Stop(Seconds(0.8));

    //Conifgure tracing of all enqueue, dequeue, and NetDevice receive event.
    //Trace output will be sent to the file "ofpmipv6-trace.tr"
    NS_LOG_INFO("Configure Tracing.");
    AsciiTraceHelper ascii;
    csma.EnableAsciiAll(ascii.CreateFileStream("ofpmipv6-trace.tr"));

    //Configure some tcodump traces; each interface will be traced.
    //The output files will be named:
    //  ofpmipv6-switch-<nodeID>-<interfaceID>.pcap
    csma.EnablePcapAll("ofpmipv6", false);

    //Create the animation object and configure for specified output
    AnimationInterface anim(animFile);
    anim.EnablePacketMetadata();


    //Strat simulation.
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop(Seconds(5.5));
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");
//    #else
//    NS_LOG_INFO ("NS-3 OpenFlow is not enabled. Cannot run simulation.");
//    #endif // NS3_OPENFLOW
}


