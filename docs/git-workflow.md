## Overview

This document defines the Git and GitHub workflow for the EduGame Engine project. Following these rules ensures a clean history, smooth collaboration, and efficient development process.

## Branch Structure

### Permanent Branches

#### `main` Branch
- **Purpose:** Production-ready, stable releases
- **Rules:**
  - Always compiles without warnings
  - All tests pass
  - Documentation is up-to-date
  - Contains only reviewed, tested code
- **Protection:**
  - Direct commits prohibited
  - PR reviews required (minimum 1)
  - CI/CD must pass
  - No force pushes

#### `develop` Branch
- **Purpose:** Integration branch for completed features
- **Rules:**
  - Should always compile
  - Integration point for feature branches
  - Pre-release testing happens here
- **Protection:**
  - Direct commits prohibited
  - PR reviews required
  - CI/CD must pass

### Feature Branches

#### Naming Convention
```
type/description
```

**Types:**
- `feature/` - New functionality
- `fix/` - Bug fixes
- `docs/` - Documentation
- `refactor/` - Code restructuring
- `test/` - Test additions
- `ci/` - CI/CD changes
- `chore/` - Maintenance tasks

**Examples:**
- `feature/ecs-core`
- `fix/memory-leak-physics`
- `docs/api-reference`
- `refactor/render-pipeline`

#### Creation Rules
- Created from `develop` branch
- One branch per logical change
- Short-lived (days, not weeks)
- Descriptive names

## Development Process

### Starting Work

1. **Ensure you're on develop and synced:**
```bash
git checkout develop
git pull origin develop
```

2. **Create feature branch:**
```bash
git checkout -b feature/your-feature-name
```

### Making Changes

#### Commit Message Format
```
[AREA] Brief imperative description

Optional detailed explanation covering:
- What changed and why
- Any breaking changes
- Related issues or context
```

**Area Prefixes:**
- `[CORE]` - Core engine systems
- `[ECS]` - Entity Component System
- `[RENDER]` - Rendering system
- `[PHYSICS]` - Physics system
- `[SCRIPT]` - Scripting/Python layer
- `[LEARN]` - Learning runtime
- `[EDITOR]` - Editor application
- `[DOCS]` - Documentation
- `[CI]` - CI/CD pipelines
- `[TEST]` - Tests
- `[BUILD]` - Build system
#### Commit Best Practices
- Small, atomic commits
- One logical change per commit
- Commit early, commit often
- Test before committing
- Follow the style guide

### Completing Work

1. **Sync with latest develop:**
```bash
git fetch origin
git rebase origin/develop
```

2. **Run tests and checks:** ( coming soon)
```bash
# Format code
./tools/format.sh

# Run tests
./tools/test.sh

# Ensure everything compiles
cmake --build build --target all
```

3. **Push to remote:**
```bash
git push -u origin feature/your-feature-name
```

## Pull Request Process

### Creating a Pull Request

1. **Navigate to GitHub repository**
2. **Click "New Pull Request"**
3. **Select branches:**
   - **base:** `develop`
   - **compare:** `your-feature-branch`
4. **Fill PR template**

### PR Template Requirements

**Title:** `[TYPE] Brief description`

**Description:**
```markdown
## Description
What does this PR do? Why is it needed?

## Changes
- Change 1
- Change 2
- Change 3

## Testing
- [ ] Unit tests added/updated
- [ ] Integration tests pass
- [ ] Manual testing performed

## Documentation
- [ ] Code comments updated
- [ ] User documentation updated
- [ ] API documentation updated

## Related Issues
Fixes #123
Related to #456
```

### PR Approval Criteria
Before merging, ensure:
- [ ] All discussions resolved
- [ ] CI/CD pipeline passes
- [ ] At least 1 approval
- [ ] No merge conflicts
- [ ] Documentation updated
- [ ] Tests pass

### Merging PRs

#### Merge Strategies
- **Squash and Merge:** For feature branches (recommended)
- **Create Merge Commit:** For complex changes
- **Rebase and Merge:** For linear history (rare)

#### After Merge
1. Delete the remote branch
2. Delete local branch
3. Update local develop
```bash
git checkout develop
git pull origin develop
git branch -d feature/your-feature-name
```

## Special Workflows

### Hotfixes (Critical bugs in `main`)

1. **Create from main:**
```bash
git checkout main
git pull origin main
git checkout -b hotfix/critical-issue
```

2. **Fix the issue and test**
3. **Merge to main and develop:**
```bash
# Merge to main
git checkout main
git merge --no-ff hotfix/critical-issue
git tag vX.Y.Z
git push origin main --tags

# Merge to develop
git checkout develop
git merge hotfix/critical-issue
git push origin develop
```

4. **Delete hotfix branch**

### Large Features (Epics)
For features spanning multiple weeks:
1. Create `feature/epic-name` base branch
2. Create sub-feature branches from epic
3. Regularly merge develop into epic
4. Use draft PRs for visibility
5. Break into smaller deliverable PRs

### Documentation Changes
1. Docs-only changes can go directly to main
2. Still require PR review
3. Use `docs/` prefix for branches
4. Update table of contents if needed

## Issue Management

### Creating Issues
**Use issue templates:**
- Bug Report
- Feature Request
- Documentation Issue
- Question

**Required information:**
- Clear title and description
- Steps to reproduce (for bugs)
- Expected vs actual behavior
- Environment details

### Working on Issues

1. **Find an issue to work on:**
   - Check `good-first-issue` label
   - Look for issues assigned to you
   - Comment before starting work

2. **Get assigned:**
   - Comment on issue expressing interest
   - Wait for assignment from maintainers
   - Don't start work without assignment

3. **Link PR to issue:**
   - Reference issue in PR description
   - Use keywords: `Fixes #123`, `Closes #456`
   - GitHub will auto-link and close on merge

## Release Process

### Versioning
We follow [Semantic Versioning](https://semver.org/):
- **MAJOR:** Breaking changes
- **MINOR:** New features (backwards compatible)
- **PATCH:** Bug fixes

### Release Steps

1. **Create release branch from develop:**
```bash
git checkout develop
git checkout -b release/v1.2.0
```

2. **Finalize release:**
   - Update version numbers
   - Update changelog
   - Run full test suite
   - Create release notes

3. **Merge to main:**
```bash
git checkout main
git merge --no-ff release/v1.2.0
git tag -a v1.2.0 -m "Release v1.2.0"
git push origin main --tags
```

4. **Merge back to develop:**
```bash
git checkout develop
git merge release/v1.2.0
```

5. **Delete release branch**

## Git Best Practices

### Daily Workflow
```bash
# Start of day
git checkout develop
git pull origin develop

# Create feature branch
git checkout -b feature/task-name

# Work and commit
git add .
git commit -m "[AREA] Description"

# End of day (if not finished)
git push -u origin feature/task-name
```

### Keeping Branches Updated
```bash
# Method 1: Rebase (preferred for clean history)
git checkout feature/branch
git fetch origin
git rebase origin/develop

# Method 2: Merge
git checkout feature/branch
git fetch origin
git merge origin/develop
```

### Common Commands Reference
```bash
# See current status
git status

# See branch history
git log --oneline --graph --all

# See changes
git diff
git diff --cached

# Save work without committing
git stash
git stash pop

# Undo last commit (keep changes)
git reset --soft HEAD~1

# Clean up merged branches
git fetch --prune
git branch --merged develop | grep -v "develop" | xargs git branch -d
```

## CI/CD Integration

### Automated Checks
The following run on every PR:
1. **Formatting:** clang-format, ruff format
2. **Linting:** clang-tidy, ruff check
3. **Build:** CMake builds on all platforms
4. **Tests:** Unit and integration tests
5. **Documentation:** Build docs, check links

### Required Checks
PRs cannot be merged unless:
- ✅ All automated checks pass
- ✅ No new warnings introduced
- ✅ Code coverage maintained or improved
- ✅ Documentation builds successfully

---

*This workflow is designed to balance structure with flexibility. As the project grows, we'll refine these processes based on team feedback and project needs.*