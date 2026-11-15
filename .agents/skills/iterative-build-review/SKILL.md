---
name: iterative-build-review
description: Plan, implement, build, critically review, and iteratively refine code changes until evidence supports the intended outcome. Use when a feature, bug fix, refactor, or design requires a plan-to-build-to-critical-review loop, especially before committing or merging a non-trivial change.
---

# Iterative Build Review

Use this workflow for non-trivial code changes that need an adversarial review before handoff.

## Workflow

1. Establish the outcome, constraints, non-goals, and acceptance checks. Write or update a concise plan when the work benefits from one.
2. Inspect the affected execution path and implement the smallest coherent change.
3. Run the relevant build, formatter, static checks, and tests. Record what hardware-only validation remains.
4. Start a critical reviewer subagent. Give it the intended outcome, constraints, changed files, and verification evidence. Ask it to follow the `critical-code-critic` workflow when that skill is available.
5. Classify each finding:
   - Fix confirmed defects and high-confidence risks.
   - Add a targeted verification step when a finding needs evidence.
   - Explain and retain a deliberate trade-off only when it is within scope.
6. Rebuild and rerun affected checks after every correction.
7. Send the reviewer the updated diff and results. Repeat review and correction until it reports no remaining actionable defect, or clearly document an external or hardware-only uncertainty.
8. Summarize the implemented outcome, reviewer-driven changes, checks run, residual risks, and whether a commit is ready.

## Guardrails

- Do not treat a successful compile as proof of correct behavior.
- Keep the reviewer independent: do not tell it the expected findings or desired conclusion.
- Keep scope explicit. Do not turn review findings into unrelated rewrites.
- Preserve user changes and local configuration unless they are explicitly in scope.
- For hardware-dependent behavior, separate code evidence from device validation.
- Follow project-specific delegation rules before modifying specialist subsystems.

## Reviewer Prompt Template

```text
Review this change critically against its intended outcome.
Inspect the execution path, boundary inputs, lifecycle, and regression risks.
Report actionable findings by severity with file locations, evidence, and the smallest credible correction or verification step.
Do not edit files.
```
