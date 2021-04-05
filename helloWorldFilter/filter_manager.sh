#!/bin/bash

_cups_filter_dir="/usr/libexec/cups/filter/";
_this_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )";

_filter_name="helloWorldFilter";
_filter="${_this_dir}/${_filter_name}";


function install_filter()
{
#    if [ -z "$(grep 'Sandboxing Relaxed' /etc/cups/cups-files.conf )" ]; then
#        echo "Sandboxing Relaxed" >> /etc/cups/cups-files.conf
#    fi

    cp "${_filter}"       "${_cups_filter_dir}"

#    chmod 700 "${_cups_filter_dir}/${_filter_name}"
#    chown root:wheel "${_cups_filter_dir}/${_filter_name}"
}

#function uninstall_filter()
#{
#    line=$(grep -n 'Sandboxing Relaxed' /etc/cups/cups-files.conf | awk -F ':' '{print $1}')
#    if [ ! -z $line ]; then
#        sed -i "" "${line}d" /etc/cups/cups-files.conf
#    fi
#
#    rm -rf "${_cups_backend_dir}/${_tea4cups_bin}"
#    rm -rf "${_cups_conf_dir}/${_tea4cups_bin}.conf"
#    rm -rf "${_cups_backend_dir}/${_ep_backend_bin}"
#
#    disable_hook
#
#    rm -rf "${HOOK_DIR}"
#}

#function disable_hook()
#{
#    if grep -q $_tea4cups_wrap_prefix /etc/cups/printers.conf; then
#        sudo launchctl stop org.cups.cupsd;
#
#        #remove for each DeviceURI prepended "tea4cups://" in printers.conf
#        sed -i "" "s|$_tea4cups_wrap_prefix||g" /etc/cups/printers.conf
#
#        sudo launchctl start org.cups.cupsd;
#    fi
#}

#function enable_hook()
#{
#    #test existance of hook
#    if [ ! -f "${_cups_backend_dir}/${_ep_backend_bin}" ] || \
#       [ ! -f "${_cups_backend_dir}/${_tea4cups_bin}" ] || \
#       [ ! -f "${_cups_conf_dir}/${_tea4cups_bin}.conf" ];
#    then
#        echo "Hook misconfiguration, reinstall it"
#        echo "Disable wrapping in /etc/cups/printers.conf"
#        disable_hook
#        exit 1
#    fi
#
#    if grep DeviceURI /etc/cups/printers.conf | grep -qv "$_tea4cups_wrap_prefix";
#    then
#        sudo launchctl stop org.cups.cupsd;
#
#        #prepend for each DeviceURI "tea4cups://" in printers.conf in case it doesn't exists
#        sed -i "" "/DeviceURI $_tea4cups_wrap_prefix/!s/DeviceURI /&$_tea4cups_wrap_prefix/g" /etc/cups/printers.conf
#        sed -i "" "s|DeviceURI $_tea4cups_cnbma2_wrap_prefix|DeviceURI $_cnbma2_wrap_prefix|g" /etc/cups/printers.conf
#
#        sudo launchctl start org.cups.cupsd;
#    fi
#}

function help()
{
    echo "Usage:"
    echo "  install filter:     $0 -i"
    echo "  uninstall filter:   $0 -u"
    echo "  enable filter for printer name:     $0 -e <printer-name>";
    echo "  disable filter for printer name:    $0 -d <printer-name>";
}

#
# Command line options processing
#
while getopts "hiued" opt; do
    case $opt in
        h)
            help; exit 0
            ;;
        i)
            install_filter; exit 0
            ;;
        u)
            uninstall_filter; exit 0
            ;;
        e)
            enable_hook; exit 0
            ;;
        d)
            disable_filter; exit 0
            ;;
        \?)
            help; exit 1
            ;;
    esac
done

help

exit 0;
