pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building..'
            }
        }
        stage('Python 3.6 Linting') {
            agent {
                dockerfile { 
                    filename 'Python_3_6_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Test 1 #####'
                sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                sh 'bandit -r ./'
                echo '##### End Test 1 #####'
            }
        }
        stage('Python 3.7 Linting') {
            agent {
                dockerfile { 
                    filename 'Python_3_7_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Test 2 #####'
                sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                sh 'bandit -r ./'
                echo '##### End Test 2 #####'
            }
        }
        stage('Python 3.8 Linting') {
            agent {
                dockerfile { 
                    filename 'Python_3_8_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Test 3 #####'
                sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                sh 'bandit -r ./'
                echo '##### End Test 3 #####'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing..'
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying....'
            }
        }
    }
}
