sudo apt-get update
sudo apt-get install build-essential python-dev python-smbus
python3 -m venv venv
. venv/bin/activate
sudo pip3 install -r requirements.txt
git clone https://github.com/adafruit/Adafruit_Python_MAX31855
cp -r Adafruit_Python_MAX31855/Adafruit_MAX31855 ./
rm -rf Adafruit_Python_MAX31855/
#cp Adafruit_Python_MAX31855/setup.py ./
#sudo python3 setup.py install
#rm setup.py
#rm -rf __pycache__
