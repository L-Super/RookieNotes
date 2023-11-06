按照 UOS 商店上架规范 deb 安装包的目录结构：

```
com.uos.changxie_1.0.9_amd64
├── DEBIAN
│   ├── control
│   ├── postinst
│   └── postrm
└── opt
    └── apps
        └── com.uos.changxie
            ├── entries
            │   ├── applications
            │   │   └── com.uos.changxie.desktop
            │   └── icons
            │       └── hicolor
            │           └── scalable
            │               └── apps
            │                   └── changxie.svg
            ├── files
            │   └── changxie
            └── info
```

制作的deb安装包脚本：

```sh
# 包名
PACKAGE=com.uos.changxie
# 同包名
#EXEC=${PACKAGE}
EXEC=changxie
VERSION=${VER} # 1.0.8
SIZE=1024
EMAIL=leo@qq.com
MAINTAINER=changxie
HOMEPAGE=http://www.xxx.com
ICON=changxie.svg
SECTION=utils
DESCRIPTION=changxie
ARCH=amd64 

# 打包目录 包名：demo-1.0.0-amd64.deb 生成到 /output
OUTPUT_DIR=output/${EXEC}_${VERSION}_uos_${ARCH}
# uos 安装根目录
UOS_ROOT_DIR=opt/apps/${PACKAGE}
# uos entries目录
UOS_ENTRTIES=${UOS_ROOT_DIR}/entries
# uos files目录
UOS_FILES=${UOS_ROOT_DIR}/files
# uos info 应用描述文件
UOS_INFO=${UOS_ROOT_DIR}/info
 
#待打包可执行程序的所在位置 
EXECDIR=/media/Leou/cxClient/scripts/package/
echo "executable file path: ${EXECDIR}"

ICONDIR=installer_script
# svg建议存放路径
ICOSVG=${UOS_ENTRTIES}/icons/hicolor/scalable/apps

#获取文件大小 KB
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
    rm -rf ${OUTPUT_DIR}
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
Maintainer: ${MAINTAINER} <${EMAIL}>
Homepage: ${HOMEPAGE}    
Description: CX Document is a new generation of document collaboration platform.
 CX Document is a new generation of document collaboration platform, which includes online document editing, 
 multi person collaboration, centralized control, secure storage...
Depends: openssl

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

# 为以后新建用户添加
cp "/${UOS_ENTRTIES}/applications/${PACKAGE}.desktop" /etc/skel/Desktop/

# 刷新图标缓存、桌面数据库和mime类型数据库，防止安装后desktop文件不出现在软件列表或没有图标
update-icon-caches /usr/share/icons/hicolor || true
update-desktop-database /usr/share/applications || true
update-mime-database /usr/share/mime || true

EOF

cat  << EOF > ${OUTPUT_DIR}/DEBIAN/prerm
#!/bin/sh
echo 'Program ready for uninstallation'
killall ${EXEC} || true

EOF

# 卸载之后删除数据
cat  << EOF > ${OUTPUT_DIR}/DEBIAN/postrm
#!/bin/sh
echo 'The program has been uninstalled'

set -e

case "\$1" in
    remove)
        # 卸载操作
        find /home/* -maxdepth 3 -type d -name ChangXie -exec rm -rf {} \;
        rm -f /etc/skel/Desktop/${PACKAGE}.desktop
        ;;
    upgrade)
        # 升级操作
        ;;
    *)
        echo "未知操作: $1"
        exit 1
        ;;
esac

exit 0

EOF

# 复制程序和依赖库
cp -r ${EXECDIR}/* ${OUTPUT_DIR}/${UOS_FILES}
 
# 下面是快捷方式的文件 #gksu 这个命令可以对该程序提权 pkexec 不可以
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
Icon=/${ICOSVG}/changxie.svg
EOF

# 有些文件需要权限
chmod 775 ${OUTPUT_DIR} -R
# deb打包命令 -b == --build
dpkg -b ${OUTPUT_DIR}
}
# 在脚本中调用函数不需要小括号
make_deb
 
#end
```

将相关的图标放在根目录，可执行文件放在`./src`里，然后终端执行该sh脚本即可，会在`output`目录下生成deb安装包。

动态获取版本号，使用Python从项目工程`version.h`中读取版本号

```sh
# 从version.h获取版本号
VER=$(python -c 'import re;content=open("../src/version.h").read(); match=re.search(r"#define VERSION_STR \"(.*?)\"",content); print(match.group(1))')

echo "app version is ${VER}"
```

postinst 脚本中，为了防止以后新建用户没有桌面图标，可将 `desktop` 文件放入 `/etc/skel/Desktop/` 中：

```sh
cp "/${UOS_ENTRTIES}/applications/${PACKAGE}.desktop" /etc/skel/Desktop/
```

注意，删除时也应同步删除。

postrm脚本中，由于升级和卸载都会执行postrm脚本，所以需要加判断区分：

```sh
set -e
case "\$1" in
    remove)
        # 卸载操作
        find /home/* -maxdepth 3 -type d -name ChangXie -exec rm -rf {} \;
        rm -f /etc/skel/Desktop/${PACKAGE}.desktop
        ;;
    upgrade)
        # 升级操作
        ;;
    *)
        echo "未知操作: $1"
        exit 1
        ;;
esac
exit 0
```

