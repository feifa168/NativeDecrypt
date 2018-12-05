//
// Created by Administrator on 2018/12/5.
//

#ifndef NATIVEDECRYPT_PARAMCONFIG_HPP
#define NATIVEDECRYPT_PARAMCONFIG_HPP

#include <string>
#include <set>
using std::string;
using std::set;

class EncFile {
public:
    bool loadall;
    set<string> files;
};
class ParamConfig {
// <encrypt>
//     <src>IdsConfig.jar</src>
//     <dst>IdsConfigEnc.jar</dst>
//     <files loadall="true">
//         <file type="package">com.ids.config</file>
//         <file type="package">com.ids.copy</file>
//         <file type="package">com.ids.param</file>
//         <file type="package">com.ids.shell</file>
//         <file type="package">com.ids.syslog</file>
//         <file type="package">com.ids</file>
//     </files>
// </encrypt>
public:
    string src;
    string dst;
    EncFile encFile;
};

#endif //NATIVEDECRYPT_PARAMCONFIG_HPP
