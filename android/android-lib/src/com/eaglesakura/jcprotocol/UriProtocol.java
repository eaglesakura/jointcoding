package com.eaglesakura.jcprotocol;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

@JCClass(
         cppNamespace = "jc",
         mode = Mode.Protocol)
public interface UriProtocol {
    /**
     * アセットから読み込みを行う。
     * iOSでは特定フォルダをアセットとして扱う
     * Android / iOS
     */
    @JCField
    public static final String SCHEME_APPLI_ASSETS = "appassets://";

    /**
     * アプリのローカル保存領域から作成する
     * Android / iOS
     */
    @JCField
    public static final String SCHEME_LOCALSTRAGE = "applocal://";

    /**
     * 外部ストレージから作成する
     * iOSではアプリの特定フォルダを外部ストレージとして扱う
     */
    @JCField
    public static final String SCHEME_EXTERNALSTRAGE = "external://";

    /**
     * HTTP通信を行う
     */
    @JCField
    public static final String SCHEME_HTTP = "http://";

    /**
     * HTTPS通信を行う
     */
    @JCField
    public static final String SCHEME_HTTPS = "https://";
}
