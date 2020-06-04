import smtplib

def breach_mail(Type, Classification, Priority, Datetime, email_list):
    with smtplib.SMTP('smtp.gmail.com', 587) as smtp:
        smtp.ehlo()
        smtp.starttls()
        smtp.ehlo()

        smtp.login('licster.breach@gmail.com', 'Lic#v77zX')             

        subject = 'A breach has been detected by the Intrusion Detection System!'
        body = 'A '+ Type +' was detected at '+ Datetime +'.'
        msg = f'Subject: {subject}\n\n{body}'
        
        for email in email_list:
            smtp.sendmail('licster.breach@gmail.com', email, msg)







