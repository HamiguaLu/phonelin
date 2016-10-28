package mobitnt.net;

import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.util.Log;

import java.util.List;
import java.util.Properties;

import org.json.JSONArray;
import org.json.JSONObject;

import mobitnt.util.*;


public class MediaManager extends ReqHandler {
    static public JSONArray getData(String uri, String selection, String[] selectionArgs) {

        ContentResolver cr = EAUtil.GetContentResolver();
        if (cr == null) {
            return null;
        }

        Cursor cur = cr.query(Uri.parse(uri), null, selection, selectionArgs, null);
        if (cur == null) {
            return null;
        }

        if (!cur.moveToFirst()) {
            cur.close();
            return null;
        }

        JSONArray ret = new JSONArray();

        int iMaxCount = EADefine.PL_MAX_JSON_ITEM_COUNT;
        do {
            JSONObject o = EAUtil.Cursor2Json(cur);
            ret.put(o);
        } while (cur.moveToNext() && --iMaxCount > 0);

        cur.close();
        return ret;
    }


    public byte[] processRequest(int action, JSONObject req) throws Exception {
        try {
            String uri = "";
            if (req.has("uri")) {
                //uri = req.getString("uri");
            }

            int media_action = req.getInt("media_action");
            if (media_action == EADefine.MEDIA_ACTION_GET_AUDIO_LIST){
                uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI.toString();
            }else if (media_action == EADefine.MEDIA_ACTION_GET_VIDEO_LIST){
                uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI.toString();
            }else if (media_action == EADefine.MEDIA_ACTION_GET_IMAGE_LIST){
                uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI.toString();
            }

            int projCount = 0;
            if (req.has("projCount")) {
                projCount = req.getInt("projCount");
            }

            JSONObject proj = null;
            if (req.has("projection")) {
                proj = req.getJSONObject("projection");
            }

            String sel = req.getString("selection");
            if (req.has("selection")) {
                sel = req.getString("selection");
            }

            JSONObject arg = null;
            if (req.has("argument")) {
                arg = req.getJSONObject("argument");
            }

            int argCount = 0;
            if (req.has("argCount")) {
                argCount = req.getInt("argCount");
            }

            String sortOrder = "";
            if (req.has("sort_order")) {
                sortOrder = req.getString("sort_order");
            }


            String[] projection = null;
            if (projCount > 0) {
                projection = new String[projCount];
                for (int i = 0; i < projCount; ++i) {
                    projection[i] = proj.getString("proj" + i);
                }
            }

            if (sel != null) {
                sel = sel.trim();
                if (sel.length() == 0) {
                    sel = null;
                }
            }

            String[] selectionArgs = null;
            if (argCount > 0) {
                selectionArgs = new String[argCount];
                for (int i = 0; i < argCount; ++i) {
                    selectionArgs[i] = arg.getString("arg" + i);
                }
            }

            //String media_action = req.getString("media_action");

            JSONObject ret = new JSONObject();
            ret.put("media_action", media_action);

            JSONArray retdata = getData(uri, sel, selectionArgs);
            if (retdata != null) {
                ret.put("data", retdata);
            }

            return json2Byte(ret);
        } catch (Exception e) {
            Log.e(EADefine.TAG, "Media" + e.toString());
        }

        return genRetCode(EADefine.EA_RET_FAILED);
    }

}
