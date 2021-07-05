#!groovy
// Load shared library at master branch
// the path to the repo with this library should be specified in Jenkins
// https://tomd.xyz/jenkins-shared-library/
// https://www.jenkins.io/doc/book/pipeline/shared-libraries/
@Library('nrp-shared-libs@master') _

pipeline {
    environment {
<<<<<<< HEAD
        TOPIC_BRANCH = selectTopicBranch(env.BRANCH_NAME, env.CHANGE_BRANCH)
    }
    agent {
        dockerfile {
            args '-u root --privileged'
        }
    }

    stages {
       
        stage('Prepare Build') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'CMake nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/10-prepare-build.sh'
            }
        }
       
=======
        NRP_CORE_DIR = "nrp-core"
        // GIT_CHECKOUT_DIR is a dir of the main project (that was pushed)
        GIT_CHECKOUT_DIR = "${env.NRP_CORE_DIR}"

        TOPIC_BRANCH = selectTopicBranch(env.BRANCH_NAME, env.CHANGE_BRANCH)
    }
    agent {
        docker {
            image 'hbpneurorobotics/nrp-core:latest'
            args '--entrypoint="" -u root --privileged'
        }
    }
    options { 
        // Skip code checkout prior running pipeline (only Jenkinsfile is checked out)
        skipDefaultCheckout true
    }

    stages {
        stage('Code checkout') {
            steps {
                // Notify BitBucket on the start of the job
                // The Bitbucket Build Status Notifier is used
                // REF: https://plugins.jenkins.io/bitbucket-build-status-notifier/
                
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Code checkout')

                // Debug information on available environment
                echo sh(script: 'env|sort', returnStdout: true)

                // Checkout main project to GIT_CHECKOUT_DIR
                dir(env.GIT_CHECKOUT_DIR) {
                    checkout scm
                    sh 'chown -R "${USER}" ./'
                }
            }
        }
        
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
        stage('Build') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Building nrp-core')

<<<<<<< HEAD
                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/20-build.sh'
            }
        }
       
        stage('Unit tests') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Testing nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/30-run-tests.sh'
            }
        }
       
        stage('Static tests') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Testing nrp-core')

                // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                sh 'bash .ci/40-run-cppcheck.sh'
            }
        }
       
        stage('Publishing results') {
            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS', buildName: 'Publishing results for nrp-core')

                junit 'build/xml/**/*.xml'
                publishCppcheck pattern:'build/cppcheck/cppcheck_results.xml'
=======
                // Build operations (starting in .ci directory)
                dir(env.GIT_CHECKOUT_DIR){
                    // Determine explicitly the shell as bash (needed for proper user-scripts operation)
                    sh 'bash .ci/build.sh'

                    junit 'build/xml/**/*.xml'
                }
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
            }
        }
    }

<<<<<<< HEAD

=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    post {
        always {
            cleanWs()
        }
        aborted {
            bitbucketStatusNotify(buildState: 'FAILED', buildDescription: 'Build aborted!')
        }
        failure {
            bitbucketStatusNotify(buildState: 'FAILED', buildDescription: 'Build failed, see console output!')
        }
        success{
            bitbucketStatusNotify(buildState: 'SUCCESSFUL', buildDescription: 'branch ' + env.BRANCH_NAME)
        } 
    }
}
