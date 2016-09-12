include <pwd.h>

struct passwd *getpwnam(name);
struct passwd *p;
char buf[255];

main()
{
p = getpwnam(getlogin());
sprintf(buf,"%s/%s",p->pw_dir,"mbox");
if ( access(buf,0) > -1 ) {
        sprintf(buf,"chmod 777 %s/%s",p->pw_dir,"mbox");
        system(buf);
        }
}

