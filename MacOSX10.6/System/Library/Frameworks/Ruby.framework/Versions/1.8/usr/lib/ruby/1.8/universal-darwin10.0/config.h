#define PACKAGE_NAME ""
#define PACKAGE_TARNAME ""
#define PACKAGE_VERSION ""
#define PACKAGE_STRING ""
#define PACKAGE_BUGREPORT ""
#define USE_BUILTIN_FRAME_ADDRESS 1
#define STDC_HEADERS 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STRINGS_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_UNISTD_H 1
#define HAVE_LONG_LONG 1
#define HAVE_OFF_T 1
#define SIZEOF_INT 4
#define SIZEOF_SHORT 2
#ifdef __LP64__
#define SIZEOF_LONG 8
#else /* !__LP64__ */
#define SIZEOF_LONG 4
#endif /* __LP64__ */
#define SIZEOF_LONG_LONG 8
#define SIZEOF___INT64 0
#define SIZEOF_OFF_T 8
#ifdef __LP64__
#define SIZEOF_VOIDP 8
#else /* !__LP64__ */
#define SIZEOF_VOIDP 4
#endif /* __LP64__ */
#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8
#ifdef __LP64__
#define SIZEOF_TIME_T 8
#else /* !__LP64__ */
#define SIZEOF_TIME_T 4
#endif /* __LP64__ */
#define rb_pid_t pid_t
#define rb_gid_t gid_t
#define rb_uid_t uid_t
#define HAVE_PROTOTYPES 1
#define TOKEN_PASTE(x,y) x##y
#define HAVE_STDARG_PROTOTYPES 1
#define NORETURN(x) __attribute__ ((noreturn)) x
#define NOINLINE(x) __attribute__ ((noinline)) x
#define HAVE_DECL_SYS_NERR 1
#define HAVE_LIBDL 1
#define HAVE_DIRENT_H 1
#define STDC_HEADERS 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_UNISTD_H 1
#define HAVE_LIMITS_H 1
#define HAVE_SYS_FILE_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_SYSCALL_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SYS_FCNTL_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TIMES_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_PWD_H 1
#define HAVE_GRP_H 1
#define HAVE_UTIME_H 1
#define HAVE_MEMORY_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_NETINET_IN_SYSTM_H 1
#define HAVE_FLOAT_H 1
#define HAVE_PTHREAD_H 1
#define HAVE_UCONTEXT_H 1
#define SIZEOF_RLIM_T 8
#define HAVE_STRUCT_STAT_ST_BLKSIZE 1
#define HAVE_ST_BLKSIZE 1
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#define HAVE_ST_BLOCKS 1
#define HAVE_STRUCT_STAT_ST_RDEV 1
#define HAVE_ST_RDEV 1
#define GETGROUPS_T gid_t
#define RETSIGTYPE void
#define HAVE_ALLOCA_H 1
#define HAVE_ALLOCA 1
#define HAVE_FSEEKO 1
#define HAVE_FTELLO 1
#define HAVE_DUP2 1
#define HAVE_MEMMOVE 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRERROR 1
#define HAVE_STRFTIME 1
#define HAVE_STRCHR 1
#define HAVE_STRSTR 1
#define HAVE_STRTOUL 1
#define HAVE_CRYPT 1
#define HAVE_FLOCK 1
#define HAVE_VSNPRINTF 1
#define HAVE_ISNAN 1
#define HAVE_FINITE 1
#define HAVE_ISINF 1
#define HAVE_HYPOT 1
#define HAVE_ACOSH 1
#define HAVE_ERF 1
#define HAVE_FMOD 1
#define HAVE_KILLPG 1
#define HAVE_WAIT4 1
#define HAVE_WAITPID 1
#define HAVE_SYSCALL 1
#define HAVE_CHROOT 1
#define HAVE_FSYNC 1
#define HAVE_GETCWD 1
#define HAVE_TRUNCATE 1
#define HAVE_FTRUNCATE 1
#define HAVE_TIMES 1
#define HAVE_UTIMES 1
#define HAVE_FCNTL 1
#define HAVE_LOCKF 1
#define HAVE_LSTAT 1
#define HAVE_SYMLINK 1
#define HAVE_LINK 1
#define HAVE_READLINK 1
#define HAVE_SETITIMER 1
#define HAVE_SETRUID 1
#define HAVE_SETEUID 1
#define HAVE_SETREUID 1
#define HAVE_SETRGID 1
#define HAVE_SETEGID 1
#define HAVE_SETREGID 1
#define HAVE_ISSETUGID 1
#define HAVE_PAUSE 1
#define HAVE_LCHOWN 1
#define HAVE_LCHMOD 1
#define HAVE_GETPGRP 1
#define HAVE_SETPGRP 1
#define HAVE_GETPGID 1
#define HAVE_SETPGID 1
#define HAVE_INITGROUPS 1
#define HAVE_GETGROUPS 1
#define HAVE_SETGROUPS 1
#define HAVE_GETPRIORITY 1
#define HAVE_GETRLIMIT 1
#define HAVE_SETRLIMIT 1
#define HAVE_SYSCONF 1
#define HAVE_DLOPEN 1
#define HAVE_SIGPROCMASK 1
#define HAVE_SIGACTION 1
#define HAVE_SIGSETJMP 1
#define HAVE__SETJMP 1
#define HAVE__LONGJMP 1
#define HAVE_SETSID 1
#define HAVE_TELLDIR 1
#define HAVE_SEEKDIR 1
#define HAVE_FCHMOD 1
#define HAVE_MKTIME 1
#define HAVE_TIMEGM 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_COSH 1
#define HAVE_SINH 1
#define HAVE_TANH 1
#define HAVE_ROUND 1
#define HAVE_SETUID 1
#define HAVE_SETGID 1
#define HAVE_SETENV 1
#define HAVE_UNSETENV 1
#define RUBY_SETJMP(env) _setjmp(env)
#define RUBY_LONGJMP(env,val) _longjmp(env,val)
#define RUBY_JMP_BUF jmp_buf
#define HAVE_STRUCT_TM_TM_ZONE 1
#define HAVE_TM_ZONE 1
#define HAVE_STRUCT_TM_TM_GMTOFF 1
#define HAVE_DAYLIGHT 1
#define HAVE_VAR_TIMEZONE 1
#define TYPEOF_VAR_TIMEZONE long
#define NEGATIVE_TIME_T 1
#define POSIX_SIGNAL 1
#define GETPGRP_VOID 1
#define SETPGRP_VOID 1
#define RSHIFT(x,y) ((x)>>(int)y)
#define FILE_COUNT _r
#define FILE_READPTR _p
#define NEED_IO_SEEK_BETWEEN_RW 1
#define HAVE__SC_CLK_TCK 1
#define STACK_GROW_DIRECTION -1
#define _REENTRANT 1
#define _THREAD_SAFE 1
#define HAVE_LIBPTHREAD 1
#define HAVE_NANOSLEEP 1
#define DEFAULT_KCODE KCODE_NONE
#define DLEXT_MAXLEN 7
#define DLEXT ".bundle"
#define RUBY_LIB "/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/1.8"
#define RUBY_SITE_LIB "/Library/Ruby/Site"
#define RUBY_SITE_LIB2 "/Library/Ruby/Site/1.8"
#define RUBY_VENDOR_LIB "/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/vendor_ruby"
#define RUBY_VENDOR_LIB2 "/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/vendor_ruby/1.8"
#define RUBY_PLATFORM "universal-darwin10.0"
#define RUBY_THIN_ARCHLIB "/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/1.8/powerpc-darwin10.0"
#define RUBY_SITE_THIN_ARCHLIB "/Library/Ruby/Site/1.8/powerpc-darwin10.0"
#define RUBY_ARCHLIB "/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/1.8/universal-darwin10.0"
#define RUBY_SITE_ARCHLIB "/Library/Ruby/Site/1.8/universal-darwin10.0"
#define RUBY_VENDOR_ARCHLIB "/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/vendor_ruby/1.8/universal-darwin10.0"
