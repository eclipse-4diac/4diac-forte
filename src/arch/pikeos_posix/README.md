------------------ FORTE for Posix PikeOS 4.2 ------------------
git://git.eclipse.org/gitroot/4diac/org.eclipse.4diac.forte.git
cd org.eclipse.4diac.forte
mkdir -p bin/pikeos_posix
cp ./src/arch/pikeos_posix/CMakeCache.txt bin/pikeos_posix/CMakeCache.txt
  - If you don't have this file, you should make one. You can follow the guide that explains how to build FORTE for Raspberry Pi. You have to indicate the C++ compiler for PikeOS and add the flags: DPIKEOS -fpermissive -std=c++
cd bin/pikeos
cmake-gui
  Create a new Posix/lwIP project in Codeo. Set SO_REUSE to 1 in lwipopts.h. Compile it and copy-paste the path to its folder into the appropriate field in CMake-GUI.
make
  - Now you have forte in src/. To try it out, create a new Posix Integration Project for PikeOS (4.2) in Codeo and indicate src/forte in a process. You should allow Multicast, add support to lwIP and pass parameters arv1=-c, argv2=192.168.0.3:61499, numargs=2. Moreover, lwIP support should be activated
  - In a terminal, after starting the licence server (together with interface tap0), issue "sudo route add -net 244.0.0.0 netmask 240.0.0.0 dev tap0"

Note Building: some errors are to be corrected, like adding the correct #include
  Note -std=c11: Compliance to C++11 is needed to use forte::arch::CThreadBase<pthread_t> in file src/arch/pikeos/forte_thread.cpp, since pthread_t is a struct pthread in PikeOS 4.2 and in version < C++11 templates instanciations are required to be either enumerables or integral type (i.e., basic type)