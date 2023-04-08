sudo apt-get update
sudo apt-get install build-essential python-dev python-smbus
python3 -m venv venv
. venv/bin/activate
sudo pip3 install -r requirements.txt
sudo python3 setup.py install