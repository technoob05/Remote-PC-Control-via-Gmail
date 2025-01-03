#pragma once
using namespace System;

namespace UserInfo {
    public ref class User {
    public:
        static String^ UserId;
        static String^ SenderEmail;
        static String^ ReceiverEmail;
        static String^ ApplicationKey;
        static String^ Password;

        static void SetUserInfo(String^ userId, String^ senderEmail, String^ receiverEmail, String^ applicationKey, String^ password) {
            UserId = userId;
            SenderEmail = senderEmail;
            ReceiverEmail = receiverEmail;
            ApplicationKey = applicationKey;
            Password = password;
        }
    };
}
