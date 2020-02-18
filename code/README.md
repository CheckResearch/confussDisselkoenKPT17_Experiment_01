# Additional Data

The benchmark can be run via the command:

	#./benchmark
	
With the option `-c [cylces]` you can further specify how often the algorithms should be run.

If you want to benchmark a real attack against an encryption software you can make use of the provided shell script

	#rungpg.sh
	
This script endlessly signs the file `victim.txt` in order to really detect an access of a cache set containing an RSA key.
## Prerequisites

In order to run the benchmark it first needs to be built. For that you only need to execute the following command inside the `benchark` folder:

	make all

If you want to make use of the `rungpg.sh` script you first need to install gnupg

	#sudo apt install gnupg

And generate an rsa signing key:

	#gpg --full-generate-key

