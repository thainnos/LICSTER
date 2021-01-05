pipeline {
    agent any

    stages {
        stage('Pytest on Python 3.6') {
            agent {
                dockerfile {
                    filename 'Pytest_Python.build'
                    dir 'projects/JenkinsPipeline/Dockerfiles'
                    args '--volume jenkins-pytest:/var/jenkins_home --user 0:0'
                }
            }
            steps {
                catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                    sh 'pip install -e devices/hmi/software'
                    sh 'cd devices/hmi/software; pytest'
                    sh 'pip uninstall LICSTERhmi -y'
                    sh 'find . -user root -name \'*\' | xargs chmod ugo+rw'
                }
            }
        }
        stage('Python Linting') {
            parallel {
                stage('Python 3.6') {
                    agent {
                        dockerfile { 
                            filename 'Python_3_6.build'
                            dir 'projects/JenkinsPipeline/Dockerfiles'
                            args '--volume jenkins-linting:/var/jenkins_home'
                        }
                    }
                    steps {
                        catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                            echo '##### Start Linting 3.6 #####'
                            sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                            sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                            echo '##### End Linting 3.6 #####'
                        }
                    }
                }

                stage('Python 3.7') {
                    agent {
                        dockerfile { 
                            filename 'Python_3_7.build'
                            dir 'projects/JenkinsPipeline/Dockerfiles'
                            args '--volume jenkins-linting:/var/jenkins_home'
                        }
                    }
                    steps {
                        catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                            echo '##### Start Linting 3.7 #####'
                            sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                            sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                            echo '##### End Linting 3.7 #####'
                        }
                    }
                }

                stage('Python 3.8') {
                    agent {
                        dockerfile { 
                            filename 'Python_3_8.build'
                            dir 'projects/JenkinsPipeline/Dockerfiles'
                            args '--volume jenkins-linting:/var/jenkins_home'
                        }
                    }
                    steps {
                        catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                            echo '##### Start Linting 3.8 #####'
                            sh 'flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics'
                            sh 'flake8 . --count --exit-zero --max-complexity=10 --max-line-length=127 --statistics'
                            echo '##### End Linting 3.8 #####'
                        }   
                    }
                }
            }
        }
        stage('Python Bandit') {
            parallel{
                stage('Python 3.6') {
                    agent {
                        dockerfile { 
                            filename 'Python_3_6.build'
                            dir 'projects/JenkinsPipeline/Dockerfiles'
                            args '--volume jenkins-bandit:/var/jenkins_home'
                        }
                    }
                    steps {
                        catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                            echo '##### Start Bandit 3.6 #####'
                            sh 'bandit -r ./ -x ./devices/hmi/software/tests/'
                            echo '##### End Bandit 3.6 #####'
                        }
                    }
                }

                stage('Python 3.7') {
                    agent {
                        dockerfile { 
                            filename 'Python_3_7.build'
                            dir 'projects/JenkinsPipeline/Dockerfiles'
                            args '--volume jenkins-bandit:/var/jenkins_home'
                        }
                    }
                    steps {
                        catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                            echo '##### Start Bandit 3.7 #####'
                            sh 'bandit -r ./ -x ./devices/hmi/software/tests/'
                            echo '##### End Bandit 3.7 #####'
                        }
                    }
                }

                stage('Python 3.8') {
                    agent {
                        dockerfile { 
                            filename 'Python_3_8.build'
                            dir 'projects/JenkinsPipeline/Dockerfiles'
                            args '--volume jenkins-bandit:/var/jenkins_home'
                        }
                    }
                    steps {
                        catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                            echo '##### Start Bandit 3.8 #####'
                            sh 'bandit -r ./ -x ./devices/hmi/software/tests/'
                            echo '##### End Bandit 3.8 #####'
                        }   
                    }
                }
            }
        }

        stage('Flawfinder') {
            agent {
                dockerfile {
                    filename 'Flawfinder.build'
                    dir 'projects/JenkinsPipeline/Dockerfiles'
                    args '--volume jenkins-flawfinder:/var/jenkins_home'
                }
            }
            steps {
                catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                    echo '##### Start Flawfinder #####'
                    sh 'flawfinder devices/remote_io/software/'
                    echo '##### End Flawfinder #####'
                }
            }
        }
        stage('RATS') {
            agent {
                dockerfile {
                    filename 'RATS.build'
                    dir 'projects/JenkinsPipeline/Dockerfiles'
                    args '--volume jenkins-rats:/var/jenkins_home'
                }
            }
            steps {
                catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
                    echo '##### Start RATS #####'
                    sh 'rats --resultsonly -w 3 devices/remote_io/software/Src/'
                    echo '##### End RATS #####'
                }
            }
        }
    }
}
