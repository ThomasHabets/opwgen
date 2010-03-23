/*
 * aptitude install libopie-dev
 * gcc opwgen.c -lopie && dd if=/dev/random bs=8 count=3 | ./opwgen
 */
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
#include<unistd.h>
#include<opie.h>

static const float version = 0.1f;

static ssize_t
safe_read(int fd, void *buf, size_t len)
{
	size_t l = len;
	char *p = buf;
	ssize_t ret = 0;
	while (l) {
		if (0 >= (ret = read(fd,p,l))) {
			break;
		}
		l-=ret;
		p+=ret;
	}
	if (ret < 0) {
		return ret;
	}
	return p-(char*)buf;
}

int
main(int argc, char **argv)
{
	int fd = 0;
	for(;;) {
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
			break;
		}
		printf("%s\n", opiebtoe(eng, buf));
	}
	return 0;
}
