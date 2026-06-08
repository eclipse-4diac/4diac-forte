# Contributing to 4diac FORTE

Thanks for your interest in this project.

## Project description

[Eclipse 4diac](https://eclipse.dev/4diac) is a Open Source Framework for Industrial Automation & Control. 
It is a reference implementation for the IEC 61499 standard. 
IEC 61499 defines a domain specific modeling language for developing distributed industrial control solutions.
IEC 61499 extends IEC 61131-3 by improving the encapsulation of software components for increased re-usability, providing a vendor independent format, and simplifying support for controller to controller communication.
Its distribution functionality and the inherent support for dynamic reconfiguration provide the required infrastructure for Industrie 4.0 and industrial IoT applications.

### What is 4diac FORTE
[4diac FORTE](https://eclipse.dev/4diac/4diac_forte) is a small portable implementation of an IEC 61499 runtime environment targeting small embedded control devices (16/32 Bit), implemented in C++. It supports online-reconfiguration of its applications and the real-time capable execution of all function block types provided by the IEC 61499 standard.

## Terms of Use

This repository is subject to the [Terms of Use of the Eclipse Foundation](https://www.eclipse.org/legal/termsofuse.php).

## Ways to Contribute

Contributions are welcome in many forms including:

- bug reports and issue reproduction
- code contributions
- documentation improvements
- testing and validation

See the [Eclipse 4diac contribute page](https://eclipse.dev/4diac/contribute/) for details.


## Contribution Guide

The development workflow, pull request process, commit guidelines, and testing expectations are described in the 
[Eclipse 4diac contribution guide](https://eclipse.dev/4diac/doc/development/contribute.html)

## Development Environment

To build and develop the 4diac FORTE you need:

- CMake 3.30 or newer
- C++ 20 or newer Compiler:
   - gcc 14 or newer
   - clang 19 or newer
   - MSVC 19.42 / VS 2022 17.12 or newer
- make or ninja (preferred)
- Boost.Test    

A detailed description of setting up a 4diac FORTE development environment and build 4diac FORTE from source can be found in our [development documentation](https://eclipse.dev/4diac/doc/installation/#_4diacforte)

## Eclipse Development Process

This project operates under the [Eclipse Foundation development process](https://eclipse.org/projects/dev_process) and [IP policy](https://www.eclipse.org/org/documents/Eclipse_IP_Policy.pdf).

## Eclipse Contributor Agreement

Before your contribution can be accepted you must sign the [Eclipse Contributor Agreement (ECA)](https://www.eclipse.org/legal/ECA.php)

The ECA provides the Eclipse Foundation with a permanent record that you agree that each of your contributions will comply with the commitments documented in the Developer Certificate of Origin (DCO). 
Having an ECA on file associated with the email address matching the "Author" field of your contribution's Git commits fulfills the DCO's requirement that you sign-off on your contributions.

For more information, please see the [Eclipse Committer Handbook](https://www.eclipse.org/projects/handbook/#resources-commit)


**Ensure that:**
- the email in your Git commits matches your Eclipse account
- your GitHub username is linked to your Eclipse account


## Contact

Project discussions take place via:

- [GitHub Discussions](https://github.com/eclipse-4diac/4diac-forte/discussions)
- [Eclipse 4diac mailing list](https://accounts.eclipse.org/mailing-list/4diac-dev)

