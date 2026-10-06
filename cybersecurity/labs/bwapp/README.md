# bWAPP Web Security Lab

Hands-on web application security testing performed in a controlled
bWAPP (Buggy Web Application) laboratory environment.

## Areas Practiced

- Cross-Site Scripting (XSS)
  - Reflected XSS
  - Stored XSS
  - DOM/AJAX-related XSS
  - Header/referrer-based XSS
- Local File Inclusion (LFI)
- Input validation testing
- Request/parameter manipulation
- Authentication and session-related testing
- Burp Suite interception and request analysis
- Security-level comparison and vulnerability verification

## Methodology

1. Identify the application functionality.
2. Intercept and inspect HTTP requests.
3. Identify user-controlled input.
4. Test input handling in the authorized laboratory.
5. Verify whether the input reaches a vulnerable sink.
6. Record impact and reproducibility.
7. Consider remediation and defensive controls.

All testing documented here refers to an intentionally vulnerable
training environment.
