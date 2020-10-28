# Contributing
We have a pre-commit git hook script, which runs unit tests and basic 
security tests before you commit. To use it, simply run
```
cd projects/JenkinsPipeline
chmod +x pre-commit.sh
cp pre-commit.sh ../../.git/hooks/pre-commit
```
