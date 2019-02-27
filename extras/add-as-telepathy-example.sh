#!/bin/sh

PROJECT_NAME=brain-im

PROJECT_PATH=$PROJECT_NAME
TELEPATHY_PATH=telepathy-qt

PROJECT_FULL_PATH=`pwd`/$PROJECT_PATH
TELEPATHY_FULL_PATH=`pwd`/$TELEPATHY_PATH

cd $TELEPATHY_FULL_PATH/examples

if [ -d "$PROJECT_NAME" ]; then
  echo "$PROJECT_NAME directory is already exists. Say 'y' to remove (and recreate)."
  rm -rI $PROJECT_NAME
fi

mkdir $PROJECT_NAME

if [ `grep $PROJECT_NAME CMakeLists.txt|wc -l` = '0' ]; then
  echo "add_subdirectory($PROJECT_NAME)" >> CMakeLists.txt
fi

cd $PROJECT_NAME
ln -s $PROJECT_FULL_PATH/base .
ln -s $PROJECT_FULL_PATH/qml .

cp $PROJECT_FULL_PATH/CMakeLists.txt .

# Remove all find_package TelepathyQt
sed -i '/find_package(TelepathyQt/d' CMakeLists.txt
sed -i '/set(TELEPATHY_QT_VERSION/d' CMakeLists.txt
sed -i '/set(TELEPATHY_QT_INSTALL_DIR/d' CMakeLists.txt
sed -i '/set(TELEPATHY_QT_INCLUDE_DIR/d' CMakeLists.txt
sed -i '/set(TELEPATHY_QT_LIBRARIES/d' CMakeLists.txt

# Promote the proper TelepathyQt version
sed -i '10i set(TELEPATHY_QT_VERSION ${TP_QT_MAJOR_VERSION}.${TP_QT_MINOR_VERSION}.${TP_QT_MICRO_VERSION})' CMakeLists.txt
sed -i '11i set(TELEPATHY_QT_INSTALL_DIR $TELEPATHY_FULL_PATH)' CMakeLists.txt
sed -i '12i set(TELEPATHY_QT_INCLUDE_DIR "")' CMakeLists.txt
sed -i '13i set(TELEPATHY_QT_LIBRARIES "telepathy-qt\${QT_VERSION_MAJOR}")' CMakeLists.txt

# Replace TelepathyQt CMake-found libraries with TpQt "this-project" ones.
sed -i 's/\${TELEPATHY_QT._LIBRARIES}/telepathy-qt\${QT_VERSION_MAJOR}/g' base/CMakeLists.txt

# Enable developer build
#sed -i 's/if (DEVELOPER_BUILD)/if (TRUE)/' CMakeLists.txt
