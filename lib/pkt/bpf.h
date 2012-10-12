#ifndef __BPF_H__
#define __BPF_H__	1

#define BPF_ALIGNMENT sizeof(int)
#define BPF_WORDALIGN(x) (((x)+(BPF_ALIGNMENT-1))&~(BPF_ALIGNMENT-1))

struct bpf_hdr {
	struct timeval	bh_tstamp;
	u_int	bh_caplen;
	u_int	bh_datalen;
	u_short	bh_hdrlen;
};

#endif
