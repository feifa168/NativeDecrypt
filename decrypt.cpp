//
// Created by Administrator on 2018/12/3.
//
#include <jvmti.h>
#include <iostream>
#include <cstring>

#include "ParseEncConfig.hpp"

using std::cout;
using std::endl;


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void JNICALL
ClassFile_LoadHook(
        jvmtiEnv *jvmti_env,
        JNIEnv* jni_env,
        jclass class_being_redefined,
        jobject loader,
        const char* name,
        jobject protection_domain,
        jint class_data_len,
        const unsigned char* class_data,
        jint* new_class_data_len,
        unsigned char** new_class_data
) {
    *new_class_data_len = class_data_len;
    jvmti_env->Allocate(class_data_len, new_class_data);

    unsigned char* my_data = *new_class_data;

    // 这里会加载所有的class名，所以必须判断哪些class需要解密，不能使用loadall判断
    bool encrypt = false;//ParseEncConfig::config.encFile.loadall;
    if ( (!encrypt) && (NULL != name) ) {
        for ( string file : ParseEncConfig::config.encFile.files ) {
            int len = file.length();
            while (len > 0) {
                len--;
                char &c = file[len];
                if ('.' == c) {
                    c = '/';
                }
            }

            if (0 == string(name).compare(0, file.length(), file)) {
                encrypt = true;
                break;
            }
        }
    }

    if( (NULL != name) && encrypt ) {
        for (int i = 0; i < class_data_len; ++i) {
            my_data[i] = class_data[i] ^ 0x07;
        }
    }else {
        for (int i = 0; i < class_data_len; ++i) {
            my_data[i] = class_data[i];
        }
    }
}

JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    jint jreturn = -1;
    do {
        if (NULL == options) {
            options = (char*)"";
        }
        jvmtiEnv *jvmti = NULL;
        jint rt = vm->GetEnv((void **)&jvmti, JVMTI_VERSION);
        if (rt) {
            cout << "getenv(&jvmti) fail, return is " << rt << endl;
            jreturn = rt;
            break;
        } else {
            jvmtiCapabilities capabilities;
            memset(&capabilities, 0, sizeof(capabilities));
            capabilities.can_generate_all_class_hook_events   = 1;
            capabilities.can_tag_objects                      = 1;
            capabilities.can_generate_object_free_events      = 1;
            capabilities.can_get_source_file_name             = 1;
            capabilities.can_get_line_numbers                 = 1;
            capabilities.can_generate_vm_object_alloc_events  = 1;

            //设置能获取哪些能力
            jvmtiError err = jvmti->AddCapabilities(&capabilities);
            //err = jvmti->GetCapabilities(&capabilities);
            if (jvmtiError::JVMTI_ERROR_NONE != err) {
                jreturn = err;
                cout << "AddCapabilities fail, return is " << err << endl;
                break;
            }

            // 读取配置文件
            if ( !ParseEncConfig::parse("enc_config.xml") ) {
                cout << ParseEncConfig::errMsg << endl;
                jreturn = -100;
                break;
            } else {
                string text;
                text.append("src=").append(ParseEncConfig::config.src).append("\n")
                        .append("dst=").append(ParseEncConfig::config.dst).append("\n")
                        .append("loadall=").append(std::to_string(ParseEncConfig::config.encFile.loadall)).append("\n")
                        .append("files\n");
                for (string s : ParseEncConfig::config.encFile.files) {
                    text.append("    ").append(s).append("\n");
                }
                cout << text << endl;
            }

            //设置事件回调
            jvmtiEventCallbacks callbacks;
            (void)memset(&callbacks,0, sizeof(callbacks));
            callbacks.ClassFileLoadHook = ClassFile_LoadHook;
            err = jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
            if (jvmtiError::JVMTI_ERROR_NONE != err) {
                jreturn = err;
                cout << "SetEventCallbacks fail, return is " << err << endl;
                break;
            }

            //设置事件通知
            err = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
            if(JVMTI_ERROR_NONE!=err){
                cout << "SetEventNotificationMode fail, return is " << err << endl;
                jreturn = err;
                break;
            }

            //cout << "capabilities.can_access_local_variables = " << capabilities.can_access_local_variables << endl;
            cout << " ok " << endl;
            jreturn = 0;
        }

        //cout << "Agent_OnLoad " << "options = " << options << endl;
    } while(0);

    return jreturn;
}

JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM* vm, char* options, void* reserved) {
    return 0;
}

JNIEXPORT void JNICALL
Agent_OnUnload(JavaVM *vm) {
    //cout << "---------- Agent_OnUnload ----------" << endl;
}

#ifdef __cplusplus
}
#endif  // __cplusplus
