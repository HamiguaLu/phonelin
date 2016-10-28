package mobitnt.net;

import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Properties;

import org.json.JSONArray;
import org.json.JSONObject;


import mobitnt.util.*;

/**
 * @author hamigua
 */
public class FileManager extends ReqHandler {
    static public final int MAX_FILE_COUNT = 1024;

    static public int getFileCountInFolder(String sFolderPath) {
        File file = new File(sFolderPath);

        File[] files = file.listFiles();
        if (files == null) {
            return 0;
        }

        return files.length;
    }

    public static String getRealPath(String sVirtualPath) {
        String sExternalRoot = getExternalStoragePath();
        sVirtualPath = sExternalRoot + sVirtualPath;
        sVirtualPath = sVirtualPath.replace("//", "/");
        return sVirtualPath;
    }

    public static HashSet<String> getExternalMounts() {
        String ext = getExternalStoragePath();
        final HashSet<String> out = new HashSet<String>();
        String reg = "(?i).*vold.*(vfat|ntfs|exfat|fat32|ext3|ext4).*rw.*";
        String s = "";
        try {
            final Process process = new ProcessBuilder().command("mount")
                    .redirectErrorStream(true).start();
            process.waitFor();
            final InputStream is = process.getInputStream();
            final byte[] buffer = new byte[1024];
            while (is.read(buffer) != -1) {
                s = s + new String(buffer);
            }
            is.close();
        } catch (final Exception e) {
            e.printStackTrace();
        }

        // parse output
        final String[] lines = s.split("\n");
        for (String line : lines) {
            if (!line.toLowerCase(Locale.US).contains("asec")) {
                if (line.matches(reg)) {
                    String[] parts = line.split(" ");
                    for (String part : parts) {
                        if (part.startsWith("/"))
                            if (!part.toLowerCase(Locale.US).contains("vold"))
                                out.add(part);
                    }
                }
            }
        }
        return out;
    }


    public static String getExternalStoragePath() {
        String state = android.os.Environment.getExternalStorageState();
        HashSet<String> out = new HashSet<String>();
        if (android.os.Environment.MEDIA_MOUNTED.equals(state)) {
            if (android.os.Environment.getExternalStorageDirectory().canWrite()) {
                return android.os.Environment.getExternalStorageDirectory()
                        .getPath();
            }
        }

        return "";
    }


    public JSONArray getFolderList(String sFolderPath, long iFrom) throws Exception {
        File file = new File(sFolderPath);

        File[] files = file.listFiles();
        if (files == null || files.length == 0) {
            return null;
        }

        int iFileCount = files.length;
        if (iFrom >= iFileCount) {
            return null;
        }

        int iCount = 0;

        JSONArray fileList = new JSONArray();

        for (int i = (int) iFrom; i < iFileCount; ++i) {
            JSONObject f = new JSONObject();
            f.put("name", files[i].getName());
            f.put("size", files[i].length());
            f.put("isDir", files[i].isDirectory());
            f.put("lastModified", files[i].lastModified());
            f.put("canExecute", files[i].canExecute());
            f.put("canRead", files[i].canRead());
            f.put("canWrite", files[i].canWrite());

            fileList.put(f);
            if (iCount > MAX_FILE_COUNT) {
                break;
            }
        }

        return fileList;
    }

    public boolean delFile(String sFolderPath) {
        File file = new File(sFolderPath);
        if (!file.exists()) {
            return false;
        }

        if (file.isFile()) {
            return file.delete();
        }

        File[] files = file.listFiles();
        if (files.length == 0) {
            return file.delete();
        }

        int iFileCount = files.length;
        for (int i = 0; i < iFileCount; ++i) {
            if (files[i].isDirectory()) {
                return delFile(files[i].getAbsolutePath());
            } else {
                if (!files[i].delete()) {
                    return false;
                }
            }
        }

        return file.delete();
    }


    public boolean reName(String sFileName, String sNewFileName) {
        try {
            File resFile = new File(sFileName);
            if (!resFile.exists()) {
                return false;
            }

            return resFile.renameTo(new File(sNewFileName));
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return false;
    }

    synchronized static public boolean createFile(String sFileName, boolean isDir) {
        try {
            File resFile = new File(sFileName);
            if (resFile.exists()) {
                return false;
            }

            if (isDir)
                return resFile.mkdir();

            return resFile.createNewFile();


        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return false;
    }

    static public FileInputStream getFile(String sFileName) {
        try {
            File resFile = new File(sFileName);
            if (!resFile.exists()) {
                return null;
            }

            return new FileInputStream(resFile);
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return null;
    }


    public int copyfile(File fromFile, File toFile, Boolean rewrite) {

        if (!fromFile.exists()) {
            return EADefine.EA_RET_FAILED;
        }

        if (!fromFile.isFile()) {
            return EADefine.EA_RET_ONLY_FILE;
        }

        if (!fromFile.canRead()) {
            return EADefine.EA_RET_FAILED;
        }

        if (!toFile.getParentFile().exists()) {
            toFile.getParentFile().mkdirs();
        }

        if (toFile.exists() && rewrite) {
            toFile.delete();
        }


        try {
            java.io.FileInputStream fosfrom = new java.io.FileInputStream(fromFile);
            java.io.FileOutputStream fosto = new FileOutputStream(toFile);

            byte bt[] = new byte[1024];

            int c;

            while ((c = fosfrom.read(bt)) > 0) {
                fosto.write(bt, 0, c);
            }

            fosfrom.close();

            fosto.close();
            return EADefine.EA_RET_OK;

        } catch (Exception ex) {
            Log.e("readfile", ex.getMessage());
            return EADefine.EA_RET_FAILED;
        }

    }


    JSONArray getSDCardList() throws Exception {
        /*
		HashSet<String> sdCards = getExternalMounts();
        if (sdCards == null) {
            return null;
        }

        JSONArray list = new JSONArray();
        Iterator<String> itr = sdCards.iterator();

        while (itr.hasNext()) {
            JSONObject f = new JSONObject();
            f.put("value", itr.next().toString());
            list.put(f);

        }*/

        String extPath = getExternalStoragePath();
        JSONArray list = new JSONArray();
        JSONObject f = new JSONObject();
        f.put("path", extPath);
        f.put("property", "External");
        list.put(f);

        return list;
    }

    public byte[] processRequest(int action, JSONObject req) throws Exception {
        try {
            if (action == EADefine.PL_REQ_UPLOAD_FILE){
                return json2Byte(req);
            }
            if (action == EADefine.PL_REQ_GET_FILE_LIST) {
                String path = req.getString("path");
                long from = req.getLong("from");
                JSONArray list = getFolderList(path, from);

                JSONObject ret = new JSONObject();
                ret.put("path", path);
                if (list != null) {
                    ret.put("data", list);
                }
                return json2Byte(ret);
            }

            if (action == EADefine.PL_REQ_RENAME_FILE) {
                String path = req.getString("path");
                String newName = req.getString("newName");
                if (reName(path, newName)) {
                    return genRetCode(EADefine.EA_RET_OK);
                }

                return genRetCode(EADefine.EA_RET_FAILED);
            }

            if (action == EADefine.PL_REQ_DEL_FILE) {
                String path = req.getString("path");
                int recusive = req.getInt("recusive");
                if (delFile(path)) {
                    return genRetCode(EADefine.EA_RET_OK);
                }

                return genRetCode(EADefine.EA_RET_FAILED);
            }

            if (action == EADefine.PL_REQ_GET_SD_CARD_LIST) {
                return json2Byte(getSDCardList());
            }

            if (action == EADefine.PL_REQ_NEW_FILE) {
                String path = req.getString("path");
                boolean isDir = req.getBoolean("isDir");

                if (createFile(path, isDir)) {
                    return genRetCode(EADefine.EA_RET_OK);
                }

                return genRetCode(EADefine.EA_RET_FAILED);

            }

            if (action == EADefine.PL_REQ_COPY_FILE) {
                String path = req.getString("path");
                String dstPath = req.getString("dstPath");

                File fromFile = new File(path);

                File toFile = new File(dstPath);

                int ret = copyfile(fromFile, toFile, true);

                return genRetCode(ret);

            }

            if (action == EADefine.PL_REQ_CUT_FILE) {
                String path = req.getString("path");
                String dstPath = req.getString("dstPath");

                File fromFile = new File(path);
                File toFile = new File(dstPath);

                int ret = copyfile(fromFile, toFile, true);

                if (ret != EADefine.EA_RET_OK) {
                    return genRetCode(ret);
                }

                if (!fromFile.delete()) {
                    toFile.delete();
                    return genRetCode(EADefine.EA_RET_FAILED);
                }

                return genRetCode(EADefine.EA_RET_OK);

            }

        } catch (Exception e) {
            e.printStackTrace();
            return returnException(e.toString());
        }

        return genRetCode(EADefine.EA_RET_UNKONW_REQ);

    }


}
