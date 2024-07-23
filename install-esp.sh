sudo mkdir -p /opt/esp
cd /opt/esp
git clone -b v5.2.2 --recursive https://github.com/espressif/esp-idf.git
cd /opt/esp/esp-idf
./install.sh esp32