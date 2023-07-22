#!/bin/sh
# 使用 root 权限运行。可选 
if [ "$(id -u)" != "0" ] ; then
    sudo sh ${0}
    exit 1
fi
 
# 包名
PACKAGE=com.uos.changxie
# 同包名
#EXEC=${PACKAGE}
EXEC=changxie
VERSION=1.0.9
SIZE=1024
MAINTAINER=leo    
ICON=changxie.svg
SECTION=utils
DESCRIPTION=${PACKAGE}
ARCH=amd64 

# 打包目录 包名：demo-1.0.0-amd64.deb 生成到 /output
OUTPUT_DIR=output/${PACKAGE}_${VERSION}_${ARCH}
# uos 安装根目录
UOS_ROOT_DIR=opt/apps/${PACKAGE}
# uos entries目录
UOS_ENTRTIES=${UOS_ROOT_DIR}/entries
# uos files目录
UOS_FILES=${UOS_ROOT_DIR}/files
# uos info 应用描述文件
UOS_INFO=${UOS_ROOT_DIR}/info
 
# 待打包可执行程序的所在位置 
EXECDIR=src
ICONDIR=.
# icon svg建议存放路径
ICOSVG=${UOS_ENTRTIES}/icons/hicolor/scalable/apps

# 获取文件大小 KB
SIZE=`du -s ${EXECDIR} | awk '{print int($1)}'`

#获取系统架构 
#ARCH=$(uname -m)
#if [ ${ARCH} == "x86_64" ] ; then
#    ARCH=amd64
#elif [ ${ARCH} == "aarch64" ]; then
#    ARCH=arm64
#elif [ ${ARCH} == "mips64" ]; then
#    ARCH=mips64el
#fi


#获取当前的操作系统名称 如Ubuntu
#OS=$(awk -F= '$1=="NAME" {gsub("[\" ]", "", $2); print tolower($2)}' /etc/os-release)
#if [ "${OS}" != "${OS#neokylin}" ]; then
#    OS="deepin"
#fi

make_deb()
{
    # 先删除原先的，避免混乱
    rm -rf ${OUTPUT_DIR}
    # 创建文件夹        
    mkdir -p ${OUTPUT_DIR}/DEBIAN
    mkdir -p ${OUTPUT_DIR}/${UOS_FILES}
    mkdir -p ${OUTPUT_DIR}/${UOS_ENTRTIES}/applications
    mkdir -p ${OUTPUT_DIR}/${ICOSVG}
    # 复制图片
    cp ${ICONDIR}/${ICON} ${OUTPUT_DIR}/${ICOSVG}
 
cat  << EOF > ${OUTPUT_DIR}/DEBIAN/control
Package: ${PACKAGE}
Version: ${VERSION}
Installed-Size: ${SIZE}
Section: ${SECTION}
Priority: optional
Architecture: ${ARCH}
Maintainer: ${MAINTAINER}    
Description: ${DESCRIPTION}

EOF

cat  << EOF > ${OUTPUT_DIR}/${UOS_ROOT_DIR}/info
{ 
"appid": "${PACKAGE}",
    "name": "${EXEC}",
    "version": "${VERSION}",
    "arch": ["${ARCH}"],
    "permissions": {
        "autostart": true,
        "notification": false,
        "trayicon": false,
        "clipboard": false,
        "account": false,
        "bluetooth": false,
        "camera": false,
        "audio_record": false,
        "installed_apps": false
},
"support-plugins": [],
"plugins": []
}

EOF

cat  << EOF > ${OUTPUT_DIR}/DEBIAN/postinst
#!/bin/sh
echo 'The program has been installed'
# 为所有用户添加桌面快捷方式
for d in /home/*/Desktop; do
  cp "/${UOS_ENTRTIES}/applications/${PACKAGE}.desktop" "\$d/${PACKAGE}.desktop"
done

# 刷新图标缓存、桌面数据库和mime类型数据库，防止安装后desktop文件不出现在软件列表或没有图标
update-icon-caches /usr/share/icons/hicolor || true
update-desktop-database /usr/share/applications || true
update-mime-database /usr/share/mime || true

EOF

# cat  << EOF > ${OUTPUT_DIR}/DEBIAN/prerm
# #!/bin/sh
# echo 'Program ready for uninstallation'

# EOF

# 卸载之后删除数据
cat  << EOF > ${OUTPUT_DIR}/DEBIAN/postrm
#!/bin/sh
echo 'The program has been uninstalled'

find /home/* -maxdepth 3 -type d -name ChangXie -exec rm -rf {} \;

EOF

# 复制程序和依赖库
cp -r ${EXECDIR}/* ${OUTPUT_DIR}/${UOS_FILES}
 
# desktop文件 #gksu 这个命令可以对该程序提权 pkexec 不可以
# Icon 推荐使用相对名称以便于系统根据主题规范查找对应的图标文件
cat  << EOF  > ${OUTPUT_DIR}/${UOS_ENTRTIES}/applications/${PACKAGE}.desktop
[Desktop Entry]
Categories=Network;Office;
Name=${PACKAGE}
Name[zh_CN]=畅写文档
Keywords=deepin;uniontech;changxie
Keywords[zh_CN]=深度;统信;畅写
Type=Application
Exec=/${UOS_FILES}/${EXEC}    
Icon=changxie
EOF

# 有些文件需要权限
chmod 775 ${OUTPUT_DIR} -R
# deb打包命令 -b == --build
dpkg -b ${OUTPUT_DIR}
}

# 在脚本中调用函数不需要小括号
make_deb
 
#end