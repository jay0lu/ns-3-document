# ns-3-document

## Installitation Guide for ns-3 with Openflow Module

---
## Install Linux

To install ns-3, you need to have Linux system.
If your computer doesn't have Linux OS, you can download and install the following virtualization systems to run Linux on your computer:

>* I recommend [VirtualBox](https://www.virtualbox.org/wiki/Downloads) 
>*  [VMware Workstation](http://www.vmware.com/products/workstation/)

Then download [Ubuntu 14.04 LTS](http://www.ubuntu.com/download/desktop) and install it in your virtual machine.

---
## Install prerequisites
Here is a detailed preinstallation guide for [ns-3 wiki installation](https://www.nsnam.org/wiki/Installation). In that detaile guide, you can choose the library you need for ns-3. Or you can install then all with the following command:
```
$ sudo apt-get install gcc g++ python python-dev mercurial bzr gdb valgrind gsl-bin libgsl0-dev libgsl0ldbl flex bison tcpdump sqlite sqlite3 libsqlite3-dev libxml2 libxml2-dev libgtk2.0-0 libgtk2.0-dev uncrustify doxygen graphviz imagemagick texlive texlive-latex-extra texlive-generic-extra texlive-generic-recommended texinfo dia texlive texlive-latex-extra texlive-extra-utils texlive-generic-recommended texi2html python-pygraphviz python-kiwi python-pygoocanvas libgoocanvas-dev python-pygccxml
```

---
## Downloading ns-3 Using Tarball

Tarball is a software archive format which could bundled multiple files.

Make a local directory for ns-3. In my case, I use the name workspace.
```
$ cd
$ mkdir workspace
$ cd workspace
```

Now you are in your ns-3 directory. Download ns-3 installation file and decompress the file.
```
$ wget http://www.nsnam.org/release/ns-allinone-3.24.tar.bz2
$ tar xjf ns-allinone-3.24.tar.bz2
```

You can check the decompressed file.
```
$ cd ns-allinone-3.24
$ ls
```

You will see:
```
bake      constants.py   ns-3.24                            README    build.py  netanim-3.106  pybindgen-0.17.0.post41+ngd10fa60  util.py
```

---
## Building ns-3

Now you can Building your ns-3. Go to your installation file:
```
$ cd
$ cd workspace/ns-allinone-3.24
```

Use `build.py` to build your ns-3.
```
$ ./build.py --enable-examples --enable-tests
```

If you see some error looks like this:
```
g++: internal compiler error: Killed (program cc1plus)
Please submit a full bug report,
with preprocessed source if appropriate.
See <file:///usr/share/doc/gcc-4.6/README.Bugs> for instructions.
Waf: Leaving directory `/home/fyang/project/workspace/repos/ns-3-allinone/ns-3-dev/build'
Build failed
 -> task in 'ns3module_lte' failed (exit status 4): 
	{task 159048492: cxx ns3module.cc -> ns3module.cc.7.o}
['/usr/bin/g++', '-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-Wno-error=deprecated-declarations', '-fstrict-alid', '-fno-strict-aliasing', '-fwrapv', '-fstack-protector', '-fno-strict-aliasing', '-fvisibility=hidden', '-Wn, '-I..', '-Isrc/lte/bindings', '-I../src/lte/bindings', '-I/usr/include/python2.7', '-I/usr/include/gtk-2.0', -I/usr/include/atk-1.0', '-I/usr/include/cairo', '-I/usr/include/gdk-pixbuf-2.0', '-I/usr/include/pango-1.0', 'glib-2.0', '-I/usr/lib/i386-linux-gnu/glib-2.0/include', '-I/usr/include/pixman-1', '-I/usr/include/freetype2',bxml2', '-DNS3_ASSERT_ENABLE', '-DNS3_LOG_ENABLE', '-DHAVE_SYS_IOCTL_H=1', '-DHAVE_IF_NETS_H=1', '-DHAVE_NET_ETTE3=1', '-DHAVE_IF_TUN_H=1', '-DHAVE_GSL=1', '-DNS_DEPRECATED=', '-DNS3_DEPRECATED_H', '-DNDEBUG', 'src/lte/binings/ns3module.cc.7.o']
Traceback (most recent call last):
  File "./build.py", line 170, in 
    sys.exit(main(sys.argv))
  File "./build.py", line 161, in main
    build_ns3(config, build_examples, build_tests, args, build_options)
  File "./build.py", line 81, in build_ns3
    run_command([sys.executable, "waf", "build"] + build_options)
  File "/home/fyang/project/workspace/repos/ns-3-allinone/util.py", line 24, in run_command
    raise CommandError("Command %r exited with code %i" % (argv, retval))
util.CommandError: Command ['/usr/bin/python', 'waf', 'build'] exited with code 1
```

My solution is to use swap.
```
sudo dd if=/dev/zero of=/swapfile bs=64M count=16
sudo mkswap /swapfile
sudo swapon /swapfile
```

Then run `build.py` again.

If you see
```
Waf: Leaving directory `/path/to/workspace/ns-allinone-3.24/ns-3.24/build'
'build' finished successfully 

Modules built:
antenna                   aodv                      applications
bridge                    buildings                 config-store
core                      csma                      csma-layout
dsdv                      dsr                       energy
fd-net-device             flow-monitor              internet
lr-wpan                   lte                       mesh
mobility                  mpi                       netanim (no Python)
network                   nix-vector-routing        olsr
point-to-point            point-to-point-layout     propagation
sixlowpan                 spectrum                  stats
tap-bridge                test (no Python)          topology-read
uan                       virtual-net-device        wave
wifi                      wimax

Modules not built (see ns-3 tutorial for explanation):
brite                     click                     openflow

Leaving directory `./ns-3.24'
```

Means you have ns-3 in your computer now. You can test with running a script:
```
$ ./waf --run hello-simulator
```

You should see the following output:
```
Hello Simulator
```

---

## Install OpenFlowSwitch module
Install necessary libraries.
```
$ sudo apt-get install libxml2 libxml2-dev
$ sudo apt-get install libboost-signals-dev libboost-filesystem-dev
```

Download OpenFlowSwitch module.
```
$ hg clone http://code.nsnam.org/jpelkey3/openflow
```

Go to 'openflow' directory, run:
```
$ cd openflow
$ ./waf configure
$ ./waf build
```

Get your openflow dirctory path.
```
$ pwd
```

Go back to ns-3 directory, link OpenFlwoSwitch module to ns-3. Replace the `path` with your path.
```
$ cd ..
$ ./waf configure --enable-examples --enable-tests --with-openflow='path/to/openflow'
```

Under `---- Summary of optional NS-3 features:`  you should see: 
```
"NS-3 OpenFlow Integration     : enabled"
```
Then run:
```
$ ./waf build
```

Now you can play with openflowswitchs :)

##NetAnim

Generate xml file:
```
$ ./waf --run "dumbbell-animation --nLeftLeaf=5 --nRightLeaf=5 --animFile=dumbbell.xml"
$ ./waf --run "grid-animation --xSize=5 --ySize=5 --animFile=grid.xml"
```

#include "ns3/netanim-module.h"
std::string animFile = "grid-animation.xml";
cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);
AnimationInterface anim (animFile);


Run NetAnim at netanim folder
```
$ ./NetAnim
```
