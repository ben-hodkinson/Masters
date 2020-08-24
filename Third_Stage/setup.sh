


# Create work directory variable (INSERT LOCATION HERE!)
export HOME_DIRECTORY="/home/matthew/Documents/Masters/Third_Stage/"

# Add source folder to the python path
MYPATH="$HOME_DIRECTORY/Source/"
[[ ":$PYTHONPATH:" != *":$MYPATH:"* ]] && PYTHONPATH=$PYTHONPATH:$MYPATH
