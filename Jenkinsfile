pipeline {
    agent any

    stages {
        stage('Python 3.6 Linting') {
            agent {
                dockerfile { 
                    filename 'Python_3_6_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Linting 3.6 #####'
                sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                echo '##### End Linting 3.6 #####'
            }
        }
        stage('Python 3.6 Bandit') {
            agent {
                dockerfile { 
                    filename 'Python_3_6_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Bandit 3.6 #####'
                sh 'bandit -r ./'
                echo '##### End Bandit 3.6 #####'
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
                echo '##### Start Linting 3.7 #####'
                sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                echo '##### End Linting 3.7 #####'
            }
        }
        stage('Python 3.7 Bandit') {
            agent {
                dockerfile { 
                    filename 'Python_3_7_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Bandit 3.7 #####'
                sh 'bandit -r ./'
                echo '##### End Bandit 3.7 #####'
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
                echo '##### Start Linting 3.8 #####'
                sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                echo '##### End Linting 3.8 #####'
            }
        }
        stage('Python 3.8 Bandit') {
            agent {
                dockerfile { 
                    filename 'Python_3_8_on_Ubuntu.build'
                    dir 'Dockerfiles'
                    args '--volume jenkins-data:/var/jenkins_home'
                }
            }
            steps {
                echo '##### Start Bandit 3.8 #####'
                sh 'bandit -r ./'
                echo '##### End Bandit 3.8 #####'
            }
        }
    }
}
