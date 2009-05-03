//
// C++ Implementation: configspamcheck
//
// Description:
//
//
// Author: Ulrich Weigelt <ulrich.weigelt@gmx.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configspamcheck.h"


typedef KGenericFactory<ConfigSpamcheck, QWidget> ConfigSpamcheckFactory;

K_EXPORT_COMPONENT_FACTORY( kcm_kshowmailconfigspamcheck, ConfigSpamcheckFactory(
    "kcm_kshowmailconfigspamcheck" ) );


ConfigSpamcheck::ConfigSpamcheck(QWidget * parent, const char * name, const QStringList & args)
  : KCModule( ConfigSpamcheckFactory::instance(), parent, args )
{
  //set the module name
  if ( !name )
    setName( "configfilter" );

  //build GUI
  //---------

  //main layout
  QVBoxLayout* layMain = new QVBoxLayout( this, 0, 10 );

  //description
  QLabel* lblDescription = new QLabel( this, "lblDescription" );
  lblDescription->setAlignment( Qt::WordBreak );
  lblDescription->setText( QString( "<i>%1</i>" ).arg( i18n( "KShowmail uses SpamAssassin to check the mails for spam. You have to install, configure and start the SpamAssassin daemon, before you can use this service." ) ) );
  lblDescription->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
  layMain->addWidget( lblDescription );

  //Test button
  btnTest = new KPushButton( KStdGuiItem::test(), this, "btnTest" );
  btnTest->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
  layMain->addWidget( btnTest );
  connect( btnTest, SIGNAL( clicked() ), this, SLOT( slotTestSpamAssassin() ) );

  //action
  gboxAction = new QGroupBox( 0, Qt::Horizontal, i18n( "Action for Spam" ), this, "gboxAction" );
  QHBoxLayout* layAction = new QHBoxLayout( gboxAction->layout(), 10 );
  layMain->addWidget( gboxAction );

  cmbAction = new KComboBox( gboxAction, "cmbAction" );
  layAction->addWidget( cmbAction );
  QToolTip::add( cmbAction, i18n( "Choose the action for spam mails." ) );
  connect( cmbAction, SIGNAL( activated( int ) ), this, SLOT( slotActionChanged( int ) ) );
  connect( cmbAction, SIGNAL( activated( int ) ), this, SLOT( slotChanged() ) );

  //insert items
  cmbAction->insertItem( i18n( "Delete" ), ID_SPAM_ACTION_BUTTONS_DELETE );
  cmbAction->insertItem( i18n( "Mark" ), ID_SPAM_ACTION_BUTTONS_MARK );
  cmbAction->insertItem( i18n( "Move" ), ID_SPAM_ACTION_BUTTONS_MOVE );

  //create edit line to defined the mailbox for move
  txtMailbox = new KLineEdit( gboxAction );
  layAction->addWidget( txtMailbox );
  connect( txtMailbox, SIGNAL( textChanged( const QString & ) ), this, SLOT( slotChanged() ) );

  //create wizard button to configure mailbox
  btnMailboxWizard= new KPushButton( KGuiItem( QString::null, "wizard" ), gboxAction );
  layAction->addWidget( btnMailboxWizard );
  QToolTip::add( btnMailboxWizard, i18n( "Choose the mailbox" ) );
  connect( btnMailboxWizard, SIGNAL( clicked() ), this, SLOT( slotOpenMailBoxWizard() ) );

  //set defaults
  switch( DEFAULT_SPAMCHECK_ACTION )
  {
    case CONFIG_VALUE_SPAMCHECK_ACTION_DELETE     : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_DELETE ); break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MARK       : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_MARK ); break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MOVE       : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_MOVE ); break;
  }

  txtMailbox->setText( DEFAULT_SPAMCHECK_ACTION_MOVE_MAILBOX );

  //enable or disable widgets
  slotActionChanged( cmbAction->currentItem() );
  gboxAction->setEnabled( isSpamAssassinRunning() );

  //get application config object (kshowmailrc)
  config = KApplication::kApplication()->config();

  //load configured values
  load();
}

ConfigSpamcheck::~ConfigSpamcheck()
{
}

void ConfigSpamcheck::load()
{
  config->setGroup( CONFIG_GROUP_SPAMCHECK );

  //load action
  switch( config->readNumEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, DEFAULT_SPAMCHECK_ACTION ) )
  {
    case CONFIG_VALUE_SPAMCHECK_ACTION_DELETE     : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_DELETE ); break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MARK       : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_MARK ); break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MOVE       : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_MOVE ); break;
  }

  //get mailbox name
  if( config->readNumEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, DEFAULT_SPAMCHECK_ACTION ) == CONFIG_VALUE_SPAMCHECK_ACTION_MOVE )
    txtMailbox->setText( config->readEntry( CONFIG_ENTRY_SPAMCHECK_MOVE_MAILBOX, DEFAULT_SPAMCHECK_ACTION_MOVE_MAILBOX ) );
  else
    txtMailbox->clear();

  //enable or disable widgets for other action
  slotActionChanged( cmbAction->currentItem() );
}

void ConfigSpamcheck::defaults()
{
  switch( DEFAULT_SPAMCHECK_ACTION )
  {
    case CONFIG_VALUE_SPAMCHECK_ACTION_DELETE     : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_DELETE ); break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MARK       : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_MARK ); break;
    case CONFIG_VALUE_SPAMCHECK_ACTION_MOVE       : cmbAction->setCurrentItem( ID_SPAM_ACTION_BUTTONS_MOVE ); break;
  }

  if( DEFAULT_SPAMCHECK_ACTION == CONFIG_VALUE_SPAMCHECK_ACTION_MOVE )
    txtMailbox->setText( QString( DEFAULT_SPAMCHECK_ACTION_MOVE_MAILBOX ) );
  else
    txtMailbox->clear();

  //enable or disable widgets for other action
  slotActionChanged( cmbAction->currentItem() );

  slotChanged();
}

void ConfigSpamcheck::save()
{
  config->setGroup( CONFIG_GROUP_SPAMCHECK );

  //save action
  switch( cmbAction->currentItem() )
  {
    case ID_SPAM_ACTION_BUTTONS_DELETE    : config->writeEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, CONFIG_VALUE_SPAMCHECK_ACTION_DELETE ); break;
    case ID_SPAM_ACTION_BUTTONS_MARK      : config->writeEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, CONFIG_VALUE_SPAMCHECK_ACTION_MARK ); break;
    case ID_SPAM_ACTION_BUTTONS_MOVE      : config->writeEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, CONFIG_VALUE_SPAMCHECK_ACTION_MOVE ); break;
    default                               : config->writeEntry( CONFIG_ENTRY_SPAMCHECK_ACTION, DEFAULT_SPAMCHECK_ACTION ); break;
  }

  //save mailbox name
  if( cmbAction->currentItem() == ID_SPAM_ACTION_BUTTONS_MOVE )
  {
    config->writeEntry( CONFIG_ENTRY_SPAMCHECK_MOVE_MAILBOX, txtMailbox->text() );
  }
  else
  {
    config->deleteEntry( CONFIG_ENTRY_SPAMCHECK_MOVE_MAILBOX );
  }

  config->sync();
}

void ConfigSpamcheck::slotChanged()
{
  KCModule::changed();
}

void ConfigSpamcheck::slotActionChanged( int index )
{
  switch( index )
  {
    case ID_SPAM_ACTION_BUTTONS_MOVE    : txtMailbox->setEnabled( true );
                                          if( txtMailbox->text() == "" )
                                            txtMailbox->setText( DEFAULT_FILTER_ACTION_MOVE_MAILBOX );
                                          btnMailboxWizard->setHidden( false );
                                          break;
    default                             : txtMailbox->setEnabled( false );
                                          btnMailboxWizard->setHidden( true );
                                          break;
  }
}

void ConfigSpamcheck::slotOpenMailBoxWizard( )
{
  MailBoxWizard wizard( this, "malboxwizard" );
  wizard.setCaption( i18n( "Mailbox Select" ) );
  int res = wizard.exec();

  if( res == QDialog::Accepted )
    txtMailbox->setText( wizard.getPath() );
}

bool ConfigSpamcheck::isSpamAssassinRunning( ) const
{
  FILE *read_fp;
  char buffer[ BUFSIZ + 1 ];
  int chars_read;
  bool found = false;

  memset( buffer, '\0', sizeof( buffer ) );
  read_fp = popen( "ps -eo comm", "r" );
  if( read_fp != NULL )
  {
    chars_read = fread( buffer, sizeof( char ), BUFSIZ, read_fp );
    while( chars_read > 0 )
    {
      buffer[ chars_read - 1 ] = '\0';
      QString output( buffer );
      found = output.contains( NAME_SPAMASSASSIN_DAEMON ) > 0;
      chars_read = fread( buffer, sizeof( char ), BUFSIZ, read_fp );
    }
    pclose( read_fp );
  }

  return found;
}

void ConfigSpamcheck::slotTestSpamAssassin( )
{
  if( isSpamAssassinRunning() )
  {
    KMessageBox::information( this, i18n( "SpamAssassin is running." ), i18n( "Check for SpamAssassin" ) );
    gboxAction->setEnabled( true );
  }
  else
  {
    KMessageBox::information( this, i18n( "SpamAssassin is not running." ), i18n( "Check for SpamAssassin" ) );
    gboxAction->setEnabled( false );
  }
}

#include "configspamcheck.moc"
