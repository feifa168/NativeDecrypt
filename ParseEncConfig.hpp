//
// Created by Administrator on 2018/12/5.
//

#ifndef NATIVEDECRYPT_PARSEENCCONFIG_HPP
#define NATIVEDECRYPT_PARSEENCCONFIG_HPP

#include "ParamConfig.hpp"
#include "pugixml.hpp"

#include <list>

using namespace pugi;
using std::list;

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

class ParseEncConfig {
public:
    static string errMsg;
    static ParamConfig config;

    static bool parse(string xml) {
        xml_document doc;
        xml_parse_result result = doc.load_file(xml.c_str());
        if ( status_ok != result.status ) {
            errMsg = string("load file ").append(xml).append(" fail, return is ").append(std::to_string(result.status));
            return false;
        }

        xpath_node ndSrc = doc.select_node("/encrypt/src");
        xpath_node ndDst = doc.select_node("/encrypt/dst");
        xpath_node ndFiles = doc.select_node("/encrypt/files");
        xpath_node_set ndLstFiles;

        parseField(ndSrc, "", config.src);
        parseField(ndDst, "", config.dst);
        parseNodeAttribute(ndFiles, "loadall", false, config.encFile.loadall);

        if (config.encFile.loadall) {
            return true;
        } else {
            ndLstFiles = ndFiles.node().select_nodes ("file[@type=\"package\"]");
            if ( parsePackages(ndLstFiles) ) {
                return true;
            }
        }

        return false;
    }

private:
    static void parseField(xpath_node nd, string defaultValue, string &ret) {
        xml_node node = nd.node();
        string s = node.text().get();
        ret = s.length()==0 ? defaultValue : s;
    }
    static void parseNodeAttribute(xpath_node nd, string attrName, bool defaultValue, bool &ret) {
        xml_node node = nd.node();
        ret = node.attribute(attrName.c_str()).as_bool(defaultValue);
    }

    static bool parsePackages(xpath_node_set items) {
        for (xpath_node nd : items) {
            xml_node node = nd.node();
            config.encFile.files.insert(node.text().get());
        }
        if (config.encFile.files.empty()) {
            errMsg = "three is no valid package config";
            return false;
        }

        return true;
    }
};

#endif //NATIVEDECRYPT_PARSEENCCONFIG_HPP
