while (true)
{
	const auto event = Term::Platform::read_raw();
	if (event.empty() && state != State::Game)
	{
		continue;
	}
	Term::Key key(event);
	if (key.type() != Term::Event::Type::Key && state != State::Game)
	{
		continue;
	}
	switch (state)
	{
		case State::Pregame:
		{
			view->PregameMessage(error);
			switch (key)
			{
			case Term::Key::One:
				time = 300;
				strategy = std::make_unique<EndlessStrategy>(height, width);
				state = State::Game;
				break;
			case Term::Key::Two:
				time = 300;
				strategy = std::make_unique<StandartStrategy>(height, width);
				state = State::Game;
				break;
			case Term::Key::Three:
				time = 150;
				strategy = std::make_unique<StandartStrategy>(height, width);
				state = State::Game;
				break;
			default:
				error = true;
			}
			break;
		}
		case State::Game:
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
			switch (key)
			{
			case Term::Key::q:
			case Term::Key::Q:
				state = State::Quit;
				break;
			case Term::Key::r:
			case Term::Key::R:
				field.Restart();
				move = Moving::Empty;
				break;
			case Term::Key::ArrowDown:
				if (move != Moving::Up)
				{
					move = Moving::Down;
				}
				break;
			case Term::Key::ArrowUp:
				if (move != Moving::Down)
				{
					move = Moving::Up;
				}
				break;
			case Term::Key::ArrowRight:
				if (move != Moving::Left)
				{
					move = Moving::Right;
				}
				break;
			case Term::Key::ArrowLeft:
				if (move != Moving::Right)
				{
					move = Moving::Left;
				}
				break;
			}
			if (move != Moving::Empty)
			{
				field.Move(state, move);
			}
			break;
		}
		case State::Quit:
		{
			std::vector<std::vector<FieldStatus>> temp;
			view->Notify(temp, state);
			switch (key)
			{
			case Term::Key::r:
			case Term::Key::R:
				field.Restart();
				move = Moving::Empty;
				state = State::Game;
				break;
			case Term::Key::q:
			case Term::Key::Q:
				break;
			default:
				Term::cout << std::endl << "Incorrect input. Please try again..." << std::endl;
			}
			break;
		}
	}
}