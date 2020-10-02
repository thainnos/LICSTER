# Jenkins Pipeline

## Docker Installation von Jenkins
Zum Erstellen von Jenkins in einem Docker Container müssen die folgenden Schritte durchgeführt werden:

1. Erstellung einer Netzwerkbrücke in Docker  

```
docker network create jenkins
```

2. Erstellen eines Volumes zum Speichern der Docker client TLS Zertifikate

```
docker volume create jenkins-docker-certs
```

3. Erstellen eines Volumes zum persistenten Speichern der Jenkins Daten 

```
docker volume create jenkins-data
```

4. Downloaden und Starten des Docker:Dind image zum Ausführen von Kommandos in Nodes/Agents

```
docker container run --name jenkins-docker --rm --detach \
--privileged --network jenkins --network-alias docker \
--env DOCKER_TLS_CERTDIR=/certs \
--volume jenkins-docker-certs:/certs/client \
--volume jenkins-data:/var/jenkins_home \
--publish 2376:2376 docker:dind 
```

5. Downloaden und Starten des jenkinsci/blueocean image     
```
docker container run --name jenkins-blueocean --rm --detach \
--network jenkins --env DOCKER_HOST=tcp://docker:2376 \
--env DOCKER_CERT_PATH=/certs/client --env DOCKER_TLS_VERIFY=1 \
--volume jenkins-data:/var/jenkins_home \
--volume jenkins-docker-certs:/certs/client:ro \
--publish 8080:8080 --publish 50000:50000 jenkinsci/blueocean
```
6.  Mit dem nachfolgenden Kommando den Jenkins Log ausgeben lassen und dann nach dem zufällig generierten Passwort suchen.
```
sudo docker exec ${CONTAINER_ID or CONTAINER_NAME} cat /var/jenkins_home/secrets/initialAdminPassword
```
7. Auf http://localhost:8080 gehen und das Passwort aus Schritt 6 eingeben.

Eine detailliertere Beschreibung der Installation ist auf [in den Jenkins Docs](https://www.jenkins.io/doc/book/installing/#setup-wizard). Außerdem sind hier auch andere Installationsarten als die Installation eines Docker Containers vorhanden.

## Jenkins initialisieren
Nachdem das Aufsetzen von Jenkins abgeschlossen ist und der Benutzer angelegt wurde, müssen die folgenden Schritte durchgeführt werden, um das LICSTER git Verzeichnis in Jenkins zu haben:
1. In der linken Leiste auf Element anlegen klicken.
2. Einen Element-Namen angeben wie z.B LICSTER-Pipeline und Multibranch Pipeline auswählen, danach auf OK klicken.
3. Den Tab Branch Sources anklicken, auf den Butten Add source klicken und Github auswählen.
4. Der Punkt Credentials hinzufügen fehlt derzeit noch!
5. Beim Punkt Repository HTTPS URL https://github.com/hsainnos/LICSTER.git einfügen.
6. Beim Punkt Behaviours > Discover Branches > Strategy im Dropdown All branches auswählen.
7. Auf Save drücken.
8. Falls This Folder is empty nach dem Auswählen der erstellten Pipeline angezeigt wird, so fehlt im angegebenen Repository noch ein Jenkinsfile.
Hinzufügen des Github Webhooks

Zum Hinzufügen eines Webhooks müssen zuerst die folgenden Schritte in Github durchgeführt werden:
1. Auf die Github Seite des Repositories gehen.
2. Auf den Tab Settings klicken.
3. In der linken Seitenleiste Webhooks auswählen.
4. Auf  Add Webhook klicken.
5. Bei Payload URL eine URL eingeben. Eine URL kann mit [ngrok](https://ngrok.com/) wie folgt kostenlos erhalten werden
6. Im Dropdown Content Type application/json auswählen.
7. Bei Which events would you like to trigger this webhook? - Just the push event auswählen.
8. Auf Add Webhook klicken.

Bei Jenkins müssen danach für das Hinzufügen des Webhooks die folgenden Schritte durchgeführt werden:
1. Auf Jenkins verwalten im linken Seitenmenü klicken.
2. Auf System konfigurieren klicken.
3. Bis zum Punkt Github nach unten scrollen.
4. Auf den Button Add Github Server klicken, danach Github Server im Dropdown auswählen.
5. Einen beliebigen Namen eingeben.
6. Bei API URL https://api.github.com eingeben.
7. Auf Manage Hooks klicken (Checkbox muss blau sein).
8. Nun auf Speichern klicken.

## UI
Durch das Auswählen von Blueocean im Menü auf der linken Seite werden Teile von Jenkins in einer moderneren Art dargestellt. Zum Beenden der Blueocean Ansicht muss man auf das Symbol links von dem Ausloggen Knopf drücken.

## Jenkins
Jenkins ist ein Server, welcher auf verschiedenen Betriebssystemen installiert werden kann. Des Weiteren kann Jenkins auch als Docker Container installiert werden. Dies ermöglicht eine praktische Kapselung und erhöhte Portabilität. Zum Testen von Jenkins und um die beim Testen verwendete Instanz von Jenkins später direkt einsetzen zu können, wurde Jenkins als Docker Container installiert.
### CI/CD Pipelines
Jenkins hat verschiedene Möglichkeiten zur Entwicklung von CI/CD Pipelines. So kann man eine Pipeline mit Blueocean, welches den Fokus auf eine leicht bedienbare und überischtliche Oberfläche setzt, mit der UI von Jenkins oder in einem Jenkinsfile, welches dem SCM hizugefügt wird entwickeln. Es kann nur eine Entwicklungsoption gewählt werden. Im Falle der CI/CD Pipeline für LICSTER war dies das Jenkinsfile.

Jenkins ermöglicht die Erstellung von Multibranch Pipelines. Jeder Branch, welcher ein Jenkinsfiile enthält, hat eine Pipeline basierend auf dem Jenkinsfile. Dies ermöglicht das Ändern  der Pipeline eines Branches ohne die Pipelines anderer Branches zu verändern. Derzeit ist in der LICSTER Jenkins Instanz nur auf dem master Branch eine Pipeline realisiert.
### Agents/Runner
Zum Durchführen der Pipeline können von Jenkins bereitgestellte Agents (,welche Runner in Gitlab entsprechen) verwendet werden oder eigene Agents erstellt werden. Zum Erstellen von Agents muss ein Dockerfile für den gewünschten Docker Container geschrieben werden und in der Jenkins Pipeline aus dem Dockerfile erstellt werden. Im Beispiel LICSTER wurde für jede Python Version ein eigener Agent erstellt und für Flawfinder ein weiterer Agent. 

### Github Einbindung
Jenkins ermöglicht die Einbindung von Github. Durch das Hinzufügen eines Webhooks auf Github wird Jenkins bei Änderungen am Repositories benachrichtigt. Was bei Änderungen am Repository durchgeführt werden soll, kann eingestellt werden. Bei der LICSTER Jenkins Instanz wird derzeit bei Veränderungen am Repository (push, pull request) die CI Pipeline gestartet.
### Stand der Pipeline
In der Jenkins Pipeline sind bisher die Bandit und Linting Test für die Python Versionen 3.6, 3.7 und 3.8 enthalten. Außerdem wurden Flawfinder und RATS der Jenkins Pipeline für C Security Tests hinzugefügt. Die Jenkins Pipeline ist eine deklarative Pipeline.
Jenkins aufsetzen

## Jenkins Pipeline Aufbau
### Stages
Eine Stage ist eine Aktion in Jenkins, welche einen eigenen Agent hat. Die Jenkins Pipeline besteht zur Zeit aus den folgenden Stages:
- Python 3.6 Linting: Der Python Quellcode wird mit flake 8 gelinted
- Python 3.6 Bandit: Statisches Sicherheitstesting des Python Quellcode
- Python 3.7 Linting: Der Python Quellcode wird mit flake 8 gelinted
- Python 3.7 Bandit: Statisches Sicherheitstesting des Python Quellcode
- Python 3.8 Linting: Der Python Quellcode wird mit flake 8 gelinted
- Python 3.8 Bandit: Statisches Sicherheitstesting des Python Quellcode
- Flawfinder: Statisches Sicherheitstesting des C Quellcode
- RATS: Statisches Sicherheitstesting des C Quellcode
### Agents
Ein Agent spezifiziert, wo eine Stage oder die gesamte Pipeline ausgeführt wird. Die Jenkins Pipeline erstellt derzeit aus den folgenden Dockerfiles Agents:
- Python_3_6_on_Ubuntu.build - Zuständig für die Python3.6 Stages
- Python_3_6_on_Ubuntu.build - Zuständig für die Python3.7 Stages
- Python_3_6_on_Ubuntu.build - Zuständig für die Python3.8 Stages
- Flawfinder.build - Zuständig für Flawfinder
- RATS.build - Zuständig für RATS