#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/skbuff.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kevin Abraham Wilson");
MODULE_DESCRIPTION("Packet Logger Kernel Module");
MODULE_VERSION("1.1");

static struct nf_hook_ops nfho; // Netfilter hook struct

// Netfilter callback
static unsigned int packet_logger_hook_func(void *priv,
                                            struct sk_buff *skb,
                                            const struct nf_hook_state *state) {
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;
    struct udphdr *udp_header;

    unsigned int src_port = 0, dst_port = 0;
    const char *proto_str = "UNKNOWN";

    if (!skb)
        return NF_ACCEPT;

    ip_header = ip_hdr(skb);
    if (!ip_header)
        return NF_ACCEPT;

    switch (ip_header->protocol) {
        case IPPROTO_TCP:
            proto_str = "TCP";
            if (skb_linearize(skb) == 0) {
                tcp_header = (struct tcphdr *)((__u32 *)ip_header + ip_header->ihl);
                src_port = ntohs(tcp_header->source);
                dst_port = ntohs(tcp_header->dest);
            }
            break;

        case IPPROTO_UDP:
            proto_str = "UDP";
            if (skb_linearize(skb) == 0) {
                udp_header = (struct udphdr *)((__u32 *)ip_header + ip_header->ihl);
                src_port = ntohs(udp_header->source);
                dst_port = ntohs(udp_header->dest);
            }
            break;

        case IPPROTO_ICMP:
            proto_str = "ICMP";
            break;

        default:
            proto_str = "OTHER";
            break;
    }

    if (src_port && dst_port)
        printk(KERN_INFO "[p_logger] %s SRC=%pI4:%u DST=%pI4:%u\n",
               proto_str, &ip_header->saddr, src_port, &ip_header->daddr, dst_port);
    else
        printk(KERN_INFO "[p_logger] %s SRC=%pI4 DST=%pI4\n",
               proto_str, &ip_header->saddr, &ip_header->daddr);

    return NF_ACCEPT;
}

// Module init
static int __init packet_logger_init(void) {
    nfho.hook = packet_logger_hook_func;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho);
    printk(KERN_INFO "[p_logger] Packet logger module loaded\n");
    return 0;
}

// Module exit
static void __exit packet_logger_exit(void) {
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "[p_logger] Packet logger module unloaded\n");
}

module_init(packet_logger_init);
module_exit(packet_logger_exit);

