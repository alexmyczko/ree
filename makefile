all:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) ree.c -o ree
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) fontdump.c -o fontdump

clean:
	rm -f fontdump ree

install:
	cp fontdump $(DESTDIR)/usr/bin
	cp ree $(DESTDIR)/usr/sbin
