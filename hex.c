/*
 * Copyright (c) 2019 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE 16

/*
 * hex -- hex viewer
 */

int
main(int argc, char *argv[])
{
	FILE *fp;
	char str[LINE + 1];
	int ch;
	size_t i, s = 0;

	if (pledge("stdio rpath", NULL) == -1)
		err(1, "pledge");

	if (argc != 2) {
		(void) fprintf(stderr, "usage: %s file\n", getprogname());
		exit(1);
	}

	if ((fp = fopen(argv[1], "r")) == NULL)
		err(1, "fopen");

	if (pledge("stdio", NULL) == -1)
		err(1, "pledge");

	(void) memset(str, 0, sizeof str);

	if ((ch = fgetc(fp)) == EOF)
		goto out;

again:
	(void) fprintf(stdout, "%08zx | ", s);
	for (i = 0; i < LINE; i++) {
		(void) fprintf(stdout, "%02x ", ch);
		if (isprint(ch))
			str[i] = (char) ch;
		else
			str[i] = '.';

		if ((ch = fgetc(fp)) == EOF) {
			while (++i < LINE) {
				(void) fputs("   ", stdout);
				str[i] = ' ';
			}
			(void) fprintf(stdout, "| %s\n", str);
			goto out;
		}
	}
	(void) fprintf(stdout, "| %s\n", str);
	s += LINE;
	goto again;

out:
	(void) fclose(fp);

	return 0;
}
