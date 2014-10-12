dnl Function to detect if libnsfdb dependencies are available
AC_DEFUN([AX_LIBNSFDB_CHECK_LOCAL],
 [dnl Check for internationalization functions in libnsfdb/libnsfdb_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

