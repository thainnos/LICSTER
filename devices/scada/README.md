# Low-cost ICS Testbed - ScadaLTS

Configure IP Adress of the Scada Raspberry Pi.
```zsh
sudo nano /etc/network/interfaces
```

The file should look like this at the end.
With this, you will have a static IP and if possible also one from DHCP. 
```
# interfaces(5) file used by ifup(8) and ifdown(8)

# Please note that this file is written to be used with dhcpcd
# For static IP, consult /etc/dhcpcd.conf and 'man dhcpcd.conf'

# Include files from /etc/network/interfaces.d:
source-directory /etc/network/interfaces.d

auto lo eth0 eth0:0
iface lo inet loopback

iface eth0 inet dhcp

iface eth0:0 inet static
    address 192.168.0.10
    netmask 255.255.255.0

```


## Install mysql-server
```zsh
sudo apt install mysql-server
```

```zsh
sudo mysql -p 
```
Enter your admin/root password of your server.

Create a sql database for ScadaLTS.
```sql
create database scadalts; 
```

Create username and password for ScadaLTS database.
```sql
CREATE USER 'scadalts' IDENTIFIED BY 'scadalts'; 
```

Give username admin rights to database.
```sql
GRANT ALL PRIVILEGES ON scadalts. * TO scadalts; 
```

Exit sql.
```sql
quit; 
```

```zsh
sudo apt install openjdk-8-jre openjdk-8-jdk
```


```zsh
sudo vim /opt/scadalts/apache-tomcat-7.0.81/webapps/ScadaBR/WEB-INF/classes/env.properties
```

```vim
db.type=mysql
db.url=jdbc:mysql://localhost:3306/scadalts
db.username=scadalts
db.password=scadalts
db.pool.maxActive=10
db.pool.maxIdle=10
```

```zsh
sudo nano /etc/init.d/tomcat
```

```bash
#!/bin/sh
# /etc/init.d/tomcat
# starts the Apache Tomcat service
### BEGIN INIT INFO
# Provides:          tomcat
# Required-Start:
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# X-Interactive:     true
# Short-Description: Start/stop tomcat application server
### END INIT INFO
 
export CATALINA_HOME="/opt/scadalts/apache-tomcat-7.0.81"
case "$1" in
start)
  if [ -f $CATALINA_HOME/bin/startup.sh ];
  then
    echo $"Starting Tomcat"
    /bin/su root $CATALINA_HOME/bin/startup.sh
  fi
  ;;
stop)
  if [ -f $CATALINA_HOME/bin/shutdown.sh ];
  then
    echo $"Stopping Tomcat"
    /bin/su root $CATALINA_HOME/bin/shutdown.sh
  fi
  ;;
*)
  echo $"Usage: $0 {start|stop}"
  exit 1
  ;;
esac

```




