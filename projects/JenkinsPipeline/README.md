# Jenkins Pipeline
- [Installation](Installation)
    - [Install Linux](Linux-Installation)
    - [Windows Installation](Windows-Installation)
- [Initialisation](Jenkins-Initialisation)
    - [Pipeline Setup](Pipeline-Setup)
    - [Adding a Github Webhook](Adding-a-Github-Webhook)
- [UI](UI)
## Installation
### Linux Installation

To install Jenkins you need to execute the following two commands:
```
# Navigate in the console to the folder containing this readme (cd 
# .../LICSTER/projects/JenkinsPipeline)
chmod +x docker_setup.sh
./docker_setup.sh
```
To continue you need to copy the last string/line the script printed,
visit http://localhost:8080 and past it into the Admin password field.

A manual for installing Jenkins can be found in the [Jenkins Docs](https://www.jenkins.io/doc/book/installing/#setup-wizard).

### Windows Installation

Will come soon.

## Jenkins Initialisation

### Pipeline Setup
After the setup of jenkins and the initialisation of the administrator the following steps need to be done to setup the Pipeline of LICSTER:
1. Click on New Item
2. Enter a name (e.g. LICSTER-Pipeline), select Multibranch Pipelione and click on OK.
3. Click on the tab Branch Sources, then click on the button Add source and on GitHub in the opened dropdown.
4. Enter https://github.com/hsainnos/LICSTER.git or the URL of your fork into the field Repository HTTPS URL.
5. Choose All branches in the Dropdown at Behaviours > Discover Branches > Strategy.
6. Click on save.

The Pipeline for LICSTER is now set up, but Jenkins won't be notified of changes to the Github repository. To use Jenkins without notifications of Github you have to click on "Scan repository now" everytime you want to start a Pipeline. 

If this is fine to you, you can proceed to the Section UI. Otherwise you can setup a webhook in Github to notify Jenkins. You will either need to have a valid URL or use ngrok (which is free to use). Ngrok forwards a port on your machine to make it publically accessible. Ngrok will provide you a different URL each time you start it, so you will have to change the Webhook URL in Github everytime you restart ngrok/your Jenkins server.

If you don't plan on running a dedicated (virtual) machine for your Jenkins server 24/7, using Webhook most likely won't benefit you.

### Adding a Github Webhook

To add a Webhook, you will first need to complete the following steps in Github:
1. Visit the Github website of the repository.
2. Click on the tab Settings (of the repository).
3. Click on Webhooks on the left side. You will be prompted to enter your password.
4. Click on "Add Webhook".
5. If you have a URL, you can skip this step. To get a URL visit [ngrok](https://ngrok.com/). Their website will tell you what you'll have to do.
6. Enter your URL in the field Payload URL in the format http://youraddress.com/github-webhook/
7. Select application/json in the Dropdown Content Type.
8. Select "Just the push event".
9. Click on Add Webhook.

Now you will need to complete the following steps in Jenkins:
1. Click on "Manage Jenkins" in the left menu.
2. Click on "Configure System".
3. Scroll down until you see the caption "GitHub".
4. Click on "Add Github Server" and choose "GitHub Server" in the dropdown.
5. Enter any name for the Github Server.
6. Click on the checkbox "Manage hooks".
7. Click on "Save".

You have successfully added a webhook. To test you webhook, you can simply make some changes in your git repository and push them to your Github Server. Remember to change the URL in Github on every restart of ngrok.

## UI
Jenkins has two interfaces, Blue Ocean and the traditional interface. Blueocean can only be used for the pipeline itself. To start Blue Ocean you simply click on "Open Blue Ocean" in the left menu. To stop the Blue Ocean UI you need to click on the symbol to the left of the logout button.