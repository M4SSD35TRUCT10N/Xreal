package xreal.client.ui.menu;

import xreal.Engine;
import xreal.client.Client;
import xreal.client.KeyCode;
import xreal.client.SoundChannel;
import xreal.client.renderer.Font;
import xreal.client.renderer.Renderer;
import xreal.client.ui.Component;
import xreal.client.ui.Cursor;
import xreal.client.ui.HorizontalAlignment;
import xreal.client.ui.Image;
import xreal.client.ui.Rectangle;
import xreal.client.ui.StackPanel;
import xreal.client.ui.UserInterface;
import xreal.client.ui.VerticalAlignment;
import xreal.client.ui.event.Event;
import xreal.client.ui.event.KeyEvent;
import xreal.client.ui.event.KeyListener;
import xreal.client.ui.event.MouseEvent;
import xreal.client.ui.event.MouseMotionListener;


/**
 * @author Robert Beckebans
 */
public class MenuFrame extends Component implements MouseMotionListener, KeyListener
{
	protected Font		fontVera;
	protected Font		fontVeraSe;
	protected Font		fontVeraBold;
	protected Font		fontVeraSerifBold;

	protected int		soundIn;
	protected int		soundMove;
	protected int		soundOut;
	protected int		soundBuzz;

	protected boolean	wrapAround;
	protected boolean	fullscreen;
	
	Image				backgroundImage;
	NavigationBar		navigationBar;

	public boolean isFullscreen()
	{
		return fullscreen;
	}

	protected boolean	showlogo;

	protected MenuFrame(String backgroundImageName)
	{
		bounds.x = 0;
		bounds.y = 0;
		bounds.width = width = UserInterface.SCREEN_WIDTH;
		bounds.height = height = UserInterface.SCREEN_HEIGHT;
		
		// background image
		backgroundImage = new Image(backgroundImageName);
		addChild(backgroundImage);

		addChild(UserInterface.getCursor());

		fontVera = Renderer.registerFont("fonts/Vera.ttf", 48);
		fontVeraSe = Renderer.registerFont("fonts/VeraSe.ttf", 48);
		fontVeraBold = Renderer.registerFont("fonts/VeraBd.ttf", 48);
		fontVeraSerifBold = Renderer.registerFont("fonts/VeraSeBd.ttf", 48);

		soundIn = Client.registerSound("sound/misc/menu1.wav");
		soundMove = Client.registerSound("sound/misc/menu2.wav");
		soundOut = Client.registerSound("sound/misc/menu3.wav");
		soundBuzz = Client.registerSound("sound/misc/menu4.wav");
		
		// navigation bar
		navigationBar = new NavigationBar();
		navigationBar.horizontalAlignment = HorizontalAlignment.Stretch;
		navigationBar.verticalAlignment = VerticalAlignment.Bottom;
		navigationBar.margin.bottom = 20;
		
		addChild(navigationBar);
	}

	@Override
	public void mouseMoved(MouseEvent e)
	{
		//Engine.println("MenuFrame.mouseMoved()");

		Cursor cursor = UserInterface.getCursor();

		// region test the active menu items
		/*
		for(Component c : children)
		{
			if(c instanceof Cursor)
				continue;

			if(c.grayed || !c.active)
				continue;

			if(!c.contains(cursor.getX(), cursor.getY()))
			{
				// cursor out of item bounds
				c.hasMouseFocus = false;
				continue;
			}

			// set focus to item at cursor
			// if(uis.activemenu->cursor != i)
			{
				// Menu_SetCursor(uis.activemenu, i);

				if(!c.grayed || c.active)
				{
					// Engine.println("item has focus at x = " + cursor.getX() + ", y = " + cursor.getY() +
					// ", component bounds = " + c.getBounds());

					// m->cursor_prev = m->cursor;
					// m->cursor = cursor;

					// Menu_CursorMoved(m);

					c.hasMouseFocus = true;
				}

				// ((menucommon_s *) (uis.activemenu->items[uis.activemenu->cursor_prev]))->flags &= ~QMF_HASMOUSEFOCUS;

				if(!c.silent)
				{
					// Client.startLocalSound(soundMove, SoundChannel.LOCAL_SOUND);
				}
			}

			// ((menucommon_s *) (uis.activemenu->items[uis.activemenu->cursor]))->flags |= QMF_HASMOUSEFOCUS;
			return;
		}
		*/
	}

	@Override
	public void keyPressed(KeyEvent e)
	{
		Engine.println("MenuFrame.keyPressed()");

		// KeyCode key = e.getKey();
		/*
		 * for(Component c : children) { if(c instanceof Cursor) continue;
		 * 
		 * if(c.hasFlags(Component.QMF_GRAYED | Component.QMF_INACTIVE)) continue;
		 * 
		 * if(!c.hasFlags(Component.QMF_HASMOUSEFOCUS)) continue;
		 * 
		 * c.processEvent(e); }
		 */
	}

	@Override
	public void keyReleased(KeyEvent e)
	{
		// Engine.println("MenuFrame.keyReleased()");
	}

	@Override
	public void processEvent(Event e)
	{
		if(e instanceof MouseEvent)
		{
			mouseMoved((MouseEvent) e);
		}
		else if(e instanceof KeyEvent)
		{
			if(((KeyEvent) e).isDown())
			{
				keyPressed((KeyEvent) e);
			}
			else
			{
				keyReleased((KeyEvent) e);
			}
		}

		// super.processEvent(e);
	}
}
