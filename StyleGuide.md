# Code Style Guide
### Formatting
- Use tabs for indentation.
- Limit line length to 100 characters.
- Use descriptive variable and function names (e.g., read_humidity_sensor()).
### Comments
- Use // for single-line comments, /* ... */ for multi-line comments.
- Document all functions with a brief description, parameters, and return value.
### File Organization
- Group related functions in .c and .h files.
### Error Handling
- Check return values for all hardware and library calls.
- Use clear error messages.
### Constants & Macros
- Use #define for hardware pin numbers and configuration values.
- Use const for immutable variables.

# Contribution Guidelines
### Getting Started
- Fork the repository and clone your fork.
- Set up the development environment as described in the README.
### Branching
- Create a new branch for each feature or bugfix
- Do not commit directly to main.
### Commits
- Write clear, concise commit messages.
- Group related changes into a single commit.
### Pull Requests
- Open a pull request (PR) against main when your feature/bugfix is ready.
- Fill out the PR template in github, describing your changes and testing steps.
- Tag relevant reviewers.
### Code Review
- Address all review comments before merging.
- Ensure your code passes all tests and builds.
### Testing
- Test your code on actual hardware before submitting.
### Documentation
- Update the README and code comments for any new features.
- Document hardware wiring changes.
