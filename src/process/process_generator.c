//
// Created by griot on 1/31/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "process.h"
#include "process_generator.h"

#define MAX_MILLISECONDS 20
#define MIN_MILLISECONDS 5

unsigned int STARTING_PROCESS_NUMBER = 0;
unsigned int nextId = 1;

const char * random_process_name () {
    const char *names[128] = {
        "explorer.exe", "svchost.exe", "cmd.exe", "notepad.exe", "chrome.exe", "msedge.exe", "firefox.exe", "powershell.exe",
        "winlogon.exe", "csrss.exe","lsass.exe", "taskhost.exe", "dwm.exe", "rundll32.exe", "services.exe","smss.exe",
        "SearchIndexer.exe", "spoolsv.exe", "wmiprvse.exe", "audiodg.exe","msiexec.exe", "SystemSettings.exe", "Taskmgr.exe",
        "ShellExperienceHost.exe", "svchost_network.exe","RuntimeBroker.exe", "mstsc.exe", "SearchProtocolHost.exe", "dllhost.exe",
        "conhost.exe","init", "systemd", "sshd", "cron", "nginx","apache2", "mysqld", "docker", "Xorg", "pulseaudio","cupsd", "dhclient",
        "NetworkManager", "rsyslogd", "gdm","snapd", "dbus-daemon", "udisksd", "gvfsd", "gvfsd-metadata","modem-manager", "polkitd",
        "bluetoothd", "kworker", "thermald","python", "java", "node", "perl", "ruby","gcc", "make", "cmake", "git", "vim","emacs", "nano",
        "code", "subl", "gimp","blender", "ffmpeg", "obs", "htop", "iotop","strace", "lsof", "netstat", "ping", "wget","traceroute",
        "scp", "rsync", "mount", "umount","parted", "fdisk", "mkfs", "e2fsck", "chkconfig","service", "systemctl", "journalctl", "kill",
        "pkill","sleep", "uptime", "ps", "jobs", "fg","bg", "killall", "nohup", "tmux", "watch","locate", "find", "grep", "sed", "awk",
        "xfce4-session", "lightdm", "sddm", "plymouth", "udisksctl","mount.ntfs", "udevd", "syslogd", "rsyslog", "acpid","cron.daily",
        "iptables", "firewalld", "auditd", "systemd-journald"
    };
    return names[rand() % 127];
}

int random_priority() {
    return (rand() % 100);
}

unsigned int random_milliseconds() {
    return (rand() % (MAX_MILLISECONDS - MIN_MILLISECONDS)) + MIN_MILLISECONDS;
}

Process * random_process(Process * parent) {
    const char * name = random_process_name();
    const unsigned int id = nextId;
    nextId++;
    const unsigned int priority = random_priority();
    const unsigned int milliseconds_remaining = random_milliseconds();
    File * reading_file = NULL;
    File * writing_file = NULL;
    Process * child = NULL;
    return create_process(id, name, parent, child, reading_file, writing_file, priority, milliseconds_remaining);
}

// int main(void) {
//     srand(time(NULL));
//     Process * parent = random_process(NULL);
//     Process * child = random_process(parent);
//     printf("generated process:%s\n", process_to_string(parent));
//     printf("generated process:%s\n", process_to_string(child));
//
//     free(child);
//     free(parent);
//     return 0;
// }