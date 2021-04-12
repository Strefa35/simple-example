#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#if 0
           struct passwd {
               char   *pw_name;       /* username */
               char   *pw_passwd;     /* user password */
               uid_t   pw_uid;        /* user ID */
               gid_t   pw_gid;        /* group ID */
               char   *pw_gecos;      /* user information */
               char   *pw_dir;        /* home directory */
               char   *pw_shell;      /* shell program */
#endif

int set_user(const char *input)
{
  int result = 1;

  if (input) {
    printf("name: %s\n", input);
    struct passwd *pw = getpwnam(input);
    if (pw) {
      int err;

      printf("  pw_name: %s\n", pw->pw_name ? pw->pw_name : "-");
      printf("pw_passwd: %s\n", pw->pw_passwd ? pw->pw_passwd : "-");
      printf("   pw_uid: %d\n", pw->pw_uid);
      printf("   pw_gid: %d\n", pw->pw_gid);
      printf(" pw_gecos: %s\n", pw->pw_gecos ? pw->pw_gecos : "-");
      printf("   pw_dir: %s\n", pw->pw_dir ? pw->pw_dir : "-");
      printf(" pw_shell: %s\n", pw->pw_shell ? pw->pw_shell : "-");

      if (1) {
        err = setuid(pw->pw_uid);
        printf("setuid(%d) - err: %d, errno: %d\n", pw->pw_uid, err, errno);
        err = setgid(pw->pw_gid);
        printf("setgid(%d) - err: %d, errno: %d\n", pw->pw_gid, err, errno);
      }
      result = 0;
    }
    else {
      int e = errno;
      printf("User '%s' not present -> Err: %d [%s]\n", input ? input : "-", e, strerror(e));
    }
  }
  return result;
}

int main(int argc, char* argv[])
{
  int result = 1;

  if (argc == 2) {
    char* user_name = argv[1];
    set_user(user_name);
    result = 0;
  }
  else {
    printf("You have to specify user name\n");
  }
  return result;
}
