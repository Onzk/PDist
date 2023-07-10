package console.java;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import pack.java.Constants;

import systeme.java.metier.CtlInterface;

public class ControleRMI {
    private ControleRMI() {
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry(Constants.CLIENT_RMI_HOST, Constants.CLIENT_RMI_PORT);
            CtlInterface stub = (CtlInterface) registry.lookup(Constants.CONNECTION_POINT);
            String message = "";
            System.out.println("[CONTROLE_RMI : INFO] : Connection etablie avec le module [SERVEUR_RMI].");
            do {
                message = stub.printInformations();
                if (message.length() > 0) {
                    System.out.println("-> " + message);
                }
            } while (message != null);
        } catch (Exception e) {
            System.err.println("[CONTROLE_RMI - RMI_ERROR] : " + e.getMessage());
        }
    }
}
