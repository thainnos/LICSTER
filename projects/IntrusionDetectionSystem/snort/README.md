# How to work with snort

Pull the git project into the directory where you want to save it.
The snort.conf file, which you are mostly going to work with, can be found in 
the snort folder. The snort.conf is already configured to use the custom rules 
for the LICSTER test bed and the Home-Net 192.168.0.0/24.

## Running snort

There are many different mods with which you can run snort, we will only
explain the most helpful mods. (For more information read the 
[snort Docommentation](https://snort-org-site.s3.amazonaws.com/production/document_files/files/000/000/249/original/snort_manual.pdf?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAIXACIED2SPMSC7GA%2F20200528%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20200528T142932Z&X-Amz-Expires=172800&X-Amz-SignedHeaders=host&X-Amz-Signature=2cfd90f9c7b195c9c0885b08a199e1bc6a79c7d9e37df9a278ea7fe0300b3857))

### Different Mods

Using the costum snort.conf
````
sudo snort -c /path/to/snort.conf
````
Running snort so that it logs to a directory.
````
sudo snort -l ./path/to/log/file -b
````
Running snort so it shoes the command in the terminal
````
sudo snort -A console
````
Telling snort to use a pcap
````
sudo snort -r /path/to/pcap/file
````
Normally you use a combination of different mods
````
sudo snort -l ./path/to/log/file -b -A console -c /path/to/snort.conf -r /path/to/pcap/file
````
