# Copilot Instructions for 4diac FORTE

## Project Overview

**Eclipse 4diac FORTE** is an open-source run-time environment for executing IEC 61499 models on a diverse set of operating systems and CPUs.
The project is primarily written in C++ with some operating-system-specific C code.
It follows the **Eclipse Foundation development process** and is licensed under **EPL 2.0**.

**Default Branch:** `develop`  
**Build Tool:** CMake  
**Key Technologies:** C++20

## Coding Guidelines

- Use descriptive names for variables, methods, and classes.
- Use established utilities or helpers to perform common tasks.
- Never include debug code, printf, or TODO comments in commits.
- Use descriptive log messages or provide user feedback where appropriate.
- Do not fail silently in case of errors.

## PR Review Guidelines

When reviewing pull requests, verify the following:

- Each PR should be focused on a single, logical topic or issue.
- Commit messages should follow the Eclipse 4diac contribution guide: https://eclipse.dev/4diac/doc/development/contribute.html
  The most important rules are:
   - Commit messages must not reference GitHub issues or pull requests.
   - Commit messages must not start with conventional commit prefixes, such as `[feat]` or `fix:`.
- Changed copyright headers should only have the year of the first contribution (e.g., `2004`), instead of a year range (e.g., `2024, 2026`).

## Dependency & Security Policies

- Avoid unintended widening of the public API surface.
- Avoid introducing new third-party dependencies without explicit approval.
- Never log sensitive info (passwords, tokens) to the console.
- Always follow good security practices.
