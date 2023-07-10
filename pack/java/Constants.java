package pack.java;

public class Constants {
    public static final String start = "\":", end = "\",";

    public static final String CLIENT_RMI_HOST = "localhost";
    public static final int CLIENT_RMI_PORT = 24500;
    public static final String CONNECTION_POINT = "rmi://" + CLIENT_RMI_HOST + ":" + CLIENT_RMI_PORT + "/CONTROLE";
    public static final int BUFFER_SIZE = 2048;
    public static final int SERVER_RMI_PORT = 9999;
    public static final int REFRESH_TIME = 500;

    public static final int MULTICAST_PORT = 8878;
    public static final String MULTICAST_HOST = "230.0.0.0";

    public static final byte NO_TEMPERATURE = -1;
    public static final String C_INFO_ID = "id";
    public static final String C_INFO_PIECE = "piece";
    public static final String C_INFO_TEMPERATURE = "temperature";
    public static final String C_INFO_VALEUR = "valeur";
    public static final String C_INFO_TYPE = "type";
}
