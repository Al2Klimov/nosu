#include <errno.h>
// errno
// EINVAL
// ENOMEM

#include <string.h>
// strerror()
// memcpy()

#include <stdio.h>
// fprintf()
// stderr

#include <stdlib.h>
// exit()
// malloc()

#include <sys/types.h>
#include <pwd.h>
// struct passwd
// getpwnam()

#include <sys/types.h>
#include <grp.h>
// struct group
// getgrnam()

#include <unistd.h>
// setgid()
// setuid()
// execvp()

#include <stddef.h>
// size_t
// NULL

static inline void report_error(char const *context) {
	int e = errno;
	if (e) {
		fprintf(stderr, "%s: %s\n", context, strerror(e));
		exit(1);
	}
}

int main(int argc, char **argv) {
	if (argc < 4) {
		fprintf(stderr, "Usage: %s USER GROUP COMMAND [ARGUMENT...]\n", argv[0]);
		exit(2);
	}

	errno = 0;
	struct passwd *usr = getpwnam(argv[1]);

	if (errno == 0 && usr == NULL) {
		errno = EINVAL;
	}

	report_error("getpwnam()");

	gid_t gid;

	if (argv[2][0]) {
		errno = 0;
		struct group *grp = getgrnam(argv[2]);

		if (errno == 0 && grp == NULL) {
			errno = EINVAL;
		}

		report_error("getgrnam()");

		gid = grp->gr_gid;
	} else {
		gid = usr->pw_gid;
	}

	errno = 0;
	setgid(gid);
	report_error("setgid()");

	errno = 0;
	setuid(usr->pw_uid);
	report_error("setuid()");

	char **args = malloc((size_t) (argc - 2) * sizeof(char *));

	if (args == NULL) {
		errno = ENOMEM;
		report_error("malloc()");
	}

	memcpy(args, argv + 3, (size_t) (argc - 3) * sizeof(char *));
	args[argc - 3] = NULL;

	errno = 0;
	execvp(args[0], args);
	report_error("execvp()");
}
