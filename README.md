#Eclipse 4diac's 4diac FORTE

[4diac FORTE](https://eclipse.dev/4diac/4diac_forte) is a run-time environment for executing IEC 61499 control applications developed with 4diac IDE. 4diac FORTE is written in C++ and highly portable. It can be compiled for many different operating systems and especially targets embedded systems like PLCs.

## License

Eclipse 4diac IDE is licensed under an [EPL 2.0](LICENSE.md).

## Contributing

We use [contribution policy](CONTRIBUTING.md), which means we can only accept contributions under
the terms of [Eclipse Contributor Agreement](http://www.eclipse.org/legal/ECA.php).


## Building 4diac FORTE

### Standard POSIX build

    ./setup_posix.sh
    cd bin/posix
    make

### Resetting CMake files

In case you ran CMake with invalid options it might help
to reset the CMake state by deleting CMake intermediate
files:

    rm -Rf CMakeCache.txt CMakeFiles/

### Building a local RPM

For building local RPMs you can use (tito)[https://github.com/dgoodwin/tito]

    tito build --test --rpm
    
    
### Building 4diac FORTE for Other systems

Detailed information on how to build 4diac FORTE for the different supported operating systems and hardware platforms can be found in our [Installation Documentation](https://github.com/eclipse-4diac/4diac-documentation/blob/main/src/installation/index.adoc)


## Links

* [Home page](https://eclipse.dev/4diac)
* [Documentation](https://github.com/eclipse-4diac/4diac-documentation/blob/main/src/index.adoc) 
* [Mailing list](https://dev.eclipse.org/mailman/listinfo/4diac-dev)
* [Wiki](https://wiki.eclipse.org/Eclipse_4diac_Wiki)
* [Examples](http://git.eclipse.org/c/4diac/org.eclipse.4diac.examples.git)

## Issue/bug trackers

* [Eclipse.org Bugzilla](https://bugs.eclipse.org/bugs/buglist.cgi?product=4diac)

