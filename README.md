# Eclipse 4diac FORTE

[![License](https://img.shields.io/badge/License-EPL%202.0-blue.svg?style=flat-square)](LICENSE.md)

[![Eclipse Foundation](https://img.shields.io/badge/Project-Eclipse%204diac-orange.svg?style=flat-square)](https://eclipse.dev/4diac/4diac)

[![Build Status](https://img.shields.io/github/actions/workflow/status/eclipse-4diac/4diac-forte/cmake-multi-platform.yml?branch=develop&style=flat-square&label=Build%20Status)](https://github.com/eclipse-4diac/4diac-forte/actions/workflows/cmake-multi-platform.yml)



Eclipse 4diac™ FORTE is a **portable, C++ run-time environment** for executing IEC 61499 control applications developed with the 4diac IDE.  
It targets a wide range of operating systems with a focus on **embedded systems** such as PLCs.

---

## Features

- Written in C++ for high performance and portability  
- Supports multiple hardware platforms and OSes  
- Designed for embedded control automation applications  
- Seamless integration with the 4diac IDE
- Implements the IEC 61499 standard for distributed control systems
- Extensible architecture for custom function blocks and protocols

---

## License

Eclipse 4diac FORTE is licensed under the [Eclipse Public License 2.0 (EPL 2.0)](LICENSE.md).

---

## Contributing

We welcome contributions! Please follow our [Contribution Policy](CONTRIBUTING.md).  
All contributions must comply with the [Eclipse Contributor Agreement (ECA)](http://www.eclipse.org/legal/ECA.php).

---

## Building 4diac FORTE
This project uses CMake for building across multiple platforms including Linux, Windows, macOS, and embedded systems using RTOS like FreeRTOS, Zephyr, ThreadX, and others.

### Standard POSIX Build

```bash
./setup_posix.sh
cd bin/posix
make
```

### Resetting CMake Cache
```bash
rm -rf CMakeCache.txt CMakeFiles/
```

### Building a Local RPM
Use tito to build local RPM packages:
```bash
tito build --test --rpm
```

### Building for Other Systems

See our Installation Documentation for detailed instructions on supported OS and hardware platforms.

# Useful Links

🌐 [Project Home](https://eclipse.dev/4diac/4diac)

🐍 [Bug Tracker](https://github.com/eclipse-4diac/4diac-forte/issues)

📚 [Documentation](https://github.com/eclipse-4diac/4diac-documentation/blob/main/src/doc_overview.adoc)  

📧 [Mailing list](https://dev.eclipse.org/mailman/listinfo/4diac-dev)

🔧 [Examples Repository](https://github.com/eclipse-4diac/4diac-examples)

