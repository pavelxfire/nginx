# NGINX source repository — agent guidance

## Build system

Custom shell-based configure + Makefile generator (NOT autotools or cmake).

```sh
auto/configure                   # generate Makefile in root
make                             # compile -> objs/nginx
sudo make install                # installs to /usr/local/nginx
```

`auto/configure` supports `--with-` and `--without-` flags for modules.
Full list: `auto/configure --help` or read `auto/options`.

Minimal dependencies:
```
apt install gcc make libpcre3-dev zlib1g-dev
```
Add `libssl-dev` for TLS (HTTP/SSL, mail/SSL, stream/SSL).

## Source layout

| Path | Purpose |
|------|---------|
| `src/core/nginx.c` | main entrypoint |
| `src/core/` | core framework (pool, buf, cycle, conf, etc.) |
| `src/event/` | event loop + I/O multiplexing (epoll, kqueue, etc.) |
| `src/http/` | HTTP core, v2 (`src/http/v2/`), v3 (`src/http/v3/`) |
| `src/http/modules/` | HTTP modules (proxy, fastcgi, etc.) |
| `src/mail/` | mail proxy (POP3/IMAP/SMTP) |
| `src/stream/` | TCP/UDP stream proxy |
| `src/os/unix/`, `src/os/win32/` | OS abstraction layer |
| `auto/` | build system scripts |
| `conf/` | default config files |
| `objs/` | build output (gitignored) |
| `docs/` | changelog XML, man page, website content |

## Version

Single source of truth: `src/core/nginx.h` — defines `NGINX_VERSION`.

## Tests

Tests are NOT in this repo. Standalone repo:
```
git clone https://github.com/nginx/nginx-tests.git
```
Described in `CONTRIBUTING.md`.

## Commit conventions

Enforced by CI (`.github/workflows/check-commit-message.yaml`, `.github/scripts/commit-msg-check.pl`):
- Subject ≤ 72 characters
- Prefix with module area when relevant (e.g. `Core:`, `HTTP:`, `Upstream:`, `QUIC:`, `Mail:`, `Stream:`)
- Prefix capitalised, first word after prefix lower case
- Body separated from subject by a blank line
- Tags/trailers (e.g. `Closes:`) separated from body by a blank line
- Body lines ≤ 72 chars (indented lines and URLs exempt)

## PR workflow

CI checks (`.github/workflows/`):
- **Whitespace** — `git log --check` on PR commits
- **Commit messages** — lint each commit
- **Version bump** — ensure `Version bump` commit exists after latest tag
- **Full CI** — delegates to `nginx/ci-self-hosted` (runs on master + stable-1.* branches)

Required CLA: F5 CLA before merge (bot-enforced).

## Release

`misc/GNUmakefile` handles tarball/zip releases.
`docs/GNUmakefile` generates CHANGES from `docs/xml/nginx/changes.xml`.
