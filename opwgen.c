/* opwgen/opwgen.c */
/*
 * (BSD license without advertising clause below)
 *
 * Copyright (c) 2006 Thomas Habets. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<ctype.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>

char *opiebtoe(char *engout, char *c);

static const char *version = "0.2";

static ssize_t
safe_read(int fd, void *buf, size_t len)
{
	size_t l = len;
	char *p = buf;
	ssize_t ret = 0;
	while (l) {
		if (0 >= (ret = read(fd,p,l))) {
                        if (errno == EINTR) {
                                continue;
                        }
                        return len - l;
		}
		l-=ret;
		p+=ret;
	}
	if (ret < 0) {
		return ret;
	}
	return p-(char*)buf;
}

static void
strlower(char *buf)
{
        while ((*buf = tolower(*buf))) {
                buf++;
        }
}

static void
usage(const char *av0, int err)
{
        printf("opwgen %s\n"
               "Usage: %s [ -Vh ] [ -f <file> ] [ -b <bits> ]"
               " [ <passwords> ]\n"
               "\n"
               "\t-b <bits>     Bits per password (default 64)\n"
               "\t-f <file>     Random data file (default /dev/urandom)\n"
               "\t-h, --help    Show this help text\n"
               "\t-V, --version Show version\n"
               "\t<passwords>   Generate this many passwords (default 1)\n"
               "\n"
               "Report bugs to: thomas@habets.pp.se\n"
               "opwgen home page: <http://github.com/ThomasHabets/opwgen>\n"
               , version, av0);
        exit(err);
}

static void
printVersion()
{
        printf("opwgen %s\n"
               "Copyright (C) 2006-2010 Thomas Habets\n"
               "License: 3-clause BSD\n"
               "This is free software: you are free to change and "
               "redistribute it.\n"
               "There is NO WARRANTY, to the extent permitted by law.\n"
               , version);
        exit(0);
}

int
main(int argc, char **argv)
{
	int fd;
        int opt;
        int num_passwords = 1;
        int bits = 128;
        int blocks;
        int c;
        char *filename = "/dev/urandom";
        int lower = 1;

        { /* handle GNU options */
                int c;
                for (c = 1; c < argc; c++) {
                        if (!strcmp(argv[c], "--")) {
                                break;
                        } else if (!strcmp(argv[c], "--help")) {
                                usage(argv[0], 0);
                        } else if (!strcmp(argv[c], "--version")) {
                                printVersion();
                        }
                }
        }

        while ((opt = getopt(argc, argv, "hf:b:V")) != -1) {
                switch(opt) {
                case 'h':
                        usage(argv[0], 0);
                case 'V':
                        printVersion(argv[0]);
                case 'f':
                        filename = optarg;
                        break;
                case 'b':
                        bits = strtol(optarg, 0, 0);
                        if (bits <= 0 || (bits % 64)) {
                                fprintf(stderr,
                                        "%s: bits must be multiple of 64 "
                                        "and > 0\n",
                                        argv[0]);
                                return 1;
                        }
                        break;
                default:
                        fprintf(stderr, "%s: unknown option '%c'\n",
                                argv[0], opt);
                        usage(argv[0], 1);
                }
        }
        blocks = bits / 64;

        if (optind < argc) {
                num_passwords = strtol(argv[optind], 0, 0);
        }

        if (0 > (fd = open(filename, O_RDONLY))) {
                fprintf(stderr, "%s: can't open %s: %s\n",
                        argv[0],
                        filename,
                        strerror(errno));
                return 1;
        }

	for(c = 0; c < num_passwords * blocks; c++) {
		char buf[8];
		ssize_t len;
		char eng[1024]; /* only 5*6 needed */

		memset(buf, 0, sizeof(buf));
		/*
		 * replace "sizeof(buf) != " with "0 == "
		 * to be able to generate passwords with less than 64bits
		 * of entropy
		 */
		if (sizeof(buf) != (len = safe_read(fd, buf, sizeof(buf)))) {
                        fprintf(stderr, "%s: read() failed: %s",
                                argv[0], strerror(errno));
                        return 1;
		}
                opiebtoe(eng, buf);
                if (lower) {
                        strlower(eng);
                }
                printf("%s", eng);
                if (!((c+1) % blocks)) {
                        printf("\n");
                } else {
                        printf(" ");
                }
	}
	return 0;
}
