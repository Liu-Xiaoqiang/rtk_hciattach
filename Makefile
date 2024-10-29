all: rtk_hciattach btreset btwake
OBJS := hciattach.o hciattach_rtk.o hciattach_h4.o rtb_fwc.o

rtk_hciattach: $(OBJS)
	$(CC) -o rtk_hciattach $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)


btreset:
	$(CC) -o btreset btreset.c

btwake:
	$(CC) -o btwake btwake.c

clean:
	rm -f $(OBJS) rtk_hciattach btreset btwake

tags: FORCE
	ctags -R
	find ./ -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cpp" > cscope.files
	cscope -bkq -i cscope.files
PHONY += FORCE
FORCE:
