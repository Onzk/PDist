package pack.java;

import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {

    public static String message = "";

    public static String getId() {
        return parse(message, Constants.C_INFO_ID);
    }

    public static String getPiece() {
        return parse(message, Constants.C_INFO_PIECE);
    }

    public static String getTemperature() {
        return parse(message, Constants.C_INFO_TEMPERATURE);
    }

    public static String getValeur() {
        return parse(message, Constants.C_INFO_VALEUR);
    }

    public static String getType() {
        return parse(message, Constants.C_INFO_TYPE);
    }

    public static String getId(String message) {
        return parse(message, Constants.C_INFO_ID);
    }

    public static String getPiece(String message) {
        return parse(message, Constants.C_INFO_PIECE);
    }

    public static String getTemperature(String message) {
        return parse(message, Constants.C_INFO_TEMPERATURE);
    }

    public static String getValeur(String message) {
        return parse(message, Constants.C_INFO_VALEUR);
    }

    public static String getType(String message) {
        return parse(message, Constants.C_INFO_TYPE);
    }

    public static String parse(String message, String key) {
        String value = "-";
        Pattern p = Pattern.compile("\"" + key + start + ".*?" + end + "");
        Matcher m = p.matcher(message);
        if (m.find())
            value = (m.group().subSequence(key.length() + start.length(), m.group().length() - 1)).toString();
        return value.replace(":", "").replace("\"", "");
    }

    public static String parse(String key) {
        return parse(message, key);
    }

    public static String parse(List<String> keys, List<String> values) {
        String message = "{";
        String value = "";
        for (int i = 0; i < keys.size(); i++) {
            try {
                value = values.get(i);
            } catch (Exception ex) {
                value = "";
            }
            message += "\"" + keys.get(i) + start + value + end;
        }
        message = message.substring(0, message.length() - 1);
        return message + "}";
    }

    public static final String start = "\":", end = "\",";
}