#ifndef _LIBARCHIVE_WIN32_H_
#define _LIBARCHIVE_WIN32_H_

#ifdef _WIN32

typedef long ssize_t;

#ifdef open
#undef open
#endif
#define open(p, f) _open((p), (f), _S_IREAD | _S_IWRITE)

#ifdef close
#undef close
#endif
#define close(f) _close(f)

#ifdef stat
#undef stat
#endif
#define stat _stat
#define lstat(p, s) _stat((p), (s))

#ifdef strerror
#undef strerror
#endif

#ifndef S_ISBLK
#define S_ISBLK(m)  (0)
#endif
#ifndef S_ISCHR
#define S_ISCHR(m)  (((m) & (_S_IFMT)) == (_S_IFCHR))
#endif
#ifndef S_ISDIR
#define S_ISDIR(m)  (((m) & (_S_IFMT)) == (_S_IFDIR))
#endif
#ifndef S_ISFIFO
#define S_ISFIFO(m) (((m) & (_S_IFMT)) == (_S_IFIFO))
#endif
#ifndef S_ISLNK
#define S_ISLNK(m)  (0)
#endif
#ifndef S_ISREG
#define S_ISREG(m)  (((m) & (_S_IFMT)) == (_S_IFREG))
#endif
#ifndef S_ISSOCK
#define S_ISSOCK(m) (0)
#endif

#endif // _WIN32

#endif // _LIBARCHIVE_WIN32_H_
