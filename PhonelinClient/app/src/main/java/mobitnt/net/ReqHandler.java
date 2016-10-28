package mobitnt.net;


import org.json.JSONArray;
import org.json.JSONObject;


public abstract class ReqHandler {


    public static byte[] returnException(String sVal) {
        try {
            JSONObject f = new JSONObject();
            f.put("exception", sVal);
            return f.toString().getBytes("utf-8");
        } catch (Exception e) {
            return null;
        }
    }


    public static byte[] genRetCode(int iCode) {
        try {
            JSONObject f = new JSONObject();
            f.put("retCode", iCode);
            return f.toString().getBytes("utf-8");
        } catch (Exception e) {
            return null;
        }
    }


    public byte[] json2Byte(JSONArray obj) throws Exception {
        if (obj == null) {
            return null;
        }

        return obj.toString().getBytes("utf-8");
    }

    public byte[] json2Byte(JSONObject obj) throws Exception {
        if (obj == null) {
            return null;
        }

        return obj.toString().getBytes("utf-8");
    }

    public abstract byte[] processRequest(int action, JSONObject req) throws Exception;

}
