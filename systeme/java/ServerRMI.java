package systeme.java;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;
import pack.java.Constants;
import systeme.java.metier.CtlInterface;

public class ServerRMI implements CtlInterface {

    public static DatagramSocket socket = null;
    public static InetAddress server = null;

    public ServerRMI() {
    }

    public String printInformations() {
        String message = "";
        try {
            byte[] buffer = new byte[Constants.BUFFER_SIZE];
            int length = buffer.length;
            DatagramPacket dataSent = new DatagramPacket(buffer, length, server,
                    Constants.SERVER_RMI_PORT);
            DatagramSocket socket = new DatagramSocket();
            socket.send(dataSent);
            DatagramPacket dataRecieved = new DatagramPacket(new byte[length], length);
            socket.receive(dataRecieved);
            message = new String(dataRecieved.getData());
            Thread.sleep(Constants.REFRESH_TIME);
        } catch (SocketTimeoutException ex) {
            System.out.println("[SERVEUR_RMI - TIMEOUT_ERROR] : " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("[SERVEUR_RMI - CLIENT_ERROR] : " + ex.getMessage());
        } catch (InterruptedException ex) {
            System.out.println("[SERVEUR_RMI - INTERRUPTED_ERROR] : " + ex.getMessage());
        }
        return message;
    }

    public static void main(String args[]) {
        try {
            server = InetAddress.getByName("255.255.255.255");
            ServerRMI obj = new ServerRMI();
            CtlInterface stub = (CtlInterface) UnicastRemoteObject.exportObject(obj, 0);
            Registry registry = LocateRegistry.createRegistry(Constants.CLIENT_RMI_PORT);
            registry.rebind(Constants.CONNECTION_POINT, stub);
            System.err.println("[SERVEUR_RMI - INFO] : Serveur démarré sur le port - " + Constants.CLIENT_RMI_PORT);
        } catch (Exception e) {
            System.err.println("[SERVEUR_RMI - CONNECTION_ERROR] : " + e.getMessage());
        }
    }
}
