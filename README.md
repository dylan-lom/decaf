decaf
=====

Not [caffeine][1]... but it does pretty much the same thing.

Why
---

I wanted something that did the bare-minimum to stop my screen turning off.

Caffeine depends on GTK and Python, which is gross. It also spits a couple of
deprecation warnings at me when I run it. It also doesn't work unless I
manage (turn off/on) dpms myself beforehand.

Decaf disables the X screen saver and dpms (if possible) while it's running.
It stops running once it gets killed, or once data being piped to it is done
(if there ever was any).

Usage
-----

```
# Compile
cc -Wall -Wextra -pedantic -o decaf decaf.c -lX11 -lXext

# Disable screensaver until killed (ie. Ctrl+C)
./decaf

# Disable screensaver until curl finishes
curl -O http://example.com 2>&1 | ./decaf
```

References
----------
[Caffeine][1]

[1]: https://code.launchpad.net/~caffeine-developers/caffeine/main
