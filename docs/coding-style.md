
## Overview

This document defines the coding standards and style guidelines for the EduGame Engine project. Consistent style is crucial for readability, maintainability, and educational value.

## Language Standards

### C++ Standard
- **Primary:** C++20
- **Minimum:** Must compile with GCC 11+, Clang 14+, MSVC 2019+
- **Allowed:** C++20 features that are widely supported across all target platforms

### Python Standard
- **Primary:** Python 3.11+
- **Minimum:** Python 3.9+ (for wider compatibility)
- **Allowed:** Python 3.11+ features (match-case, exception groups, etc.)

## Style Enforcement Tools

### C++ Style
We will adopt a **modified Linux Kernel style** with adjustments for educational clarity:

**Base Style:** Linux Kernel Coding Style
- **Indentation:** 2 spaces (not tabs)
- **Line length:** 100 characters maximum
- **Braces:** K&R style with educational considerations
- **Naming:** Descriptive, clear names prioritizing readability over brevity

**Enforcement Tools:**
- `.clang-format` configuration file
- `clang-tidy` for static analysis
- Pre-commit hooks for automated formatting

### Python Style
We follow **strict Python conventions** with `ruff` enforcement:

**Base Style:** PEP 8 with educational adaptations
- **Formatter:** `ruff format`
- **Linter:** `ruff check`
- **Import sorter:** `ruff isort`

**Configuration:** `pyproject.toml` with project-specific rules

## Configuration Files

### C++ Configuration
- `.clang-format` - Formatting rules based on Linux kernel style
- `.clang-tidy` - Static analysis configuration
- `cmake/CodeStyle.cmake` - Build integration

### Python Configuration
- `pyproject.toml` - Ruff configuration and project settings
- `.pre-commit-config.yaml` - Pre-commit hooks for both languages

## Adoption Process
*Adoption process instructions coming soon in v0.1.0*
## Review and Updates
*Review and updates instructions coming soon in v0.1.0*
## Resources

### C++ References
- [Linux Kernel Coding Style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [clang-format documentation](https://clang.llvm.org/docs/ClangFormat.html)

### Python References
- [PEP 8 — Style Guide for Python Code](https://peps.python.org/pep-0008/)
- [Ruff Documentation](https://docs.astral.sh/ruff/)
- [Python Educator's Guide](https://www.python.org/dev/peps/pep-0572/#teaching-recommendations)

---

*This document will be updated as the project evolves and as we gather feedback from contributors and users.*